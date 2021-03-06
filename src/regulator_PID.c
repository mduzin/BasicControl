/*
 * regulator_PI.c
 *
 *  Created on: Aug 8, 2015
 *      Author: mduzinki
 */

#include "common.h"
#include "regulator_PID.h"
#include "model_1.h"
#include "time_source.h"
#include "time_observer.h"




typedef struct _REG_PID
{
   //settings
   BOOL Reg_On;      //Regulator On/Off
   BOOL P_sel;       //Proportional part is On/Off
   BOOL I_sel;       //Intergrator part is On/Off
   BOOL D_sel;       //Differential part in On/Off
   int  AntiWindup;  //AntiWindup algorithm selection
   double kp;        //gain [-]
   double Ti;        //Intergrator time [s]
   double Td;        //Differential time [s]
   double Tp;		 //Scanning period [s]
   double Tt;		 //Tracking time for antiwindup back-calculation algorithm
   double CS_min;	 //min Control Signal
   double CS_max;    //max Control Signal

   //runtime
   double P;		//Proportional value
   double I;		//Integrator value
   double D;		//Differential value
   double CS_raw;	//Raw Control Signal before saturation limit check etc.
   double CS;	    //Output Control Signal

   double e;           //error
   double es;          //error for antiwindup back-calculation algorithm
   double int_e;
   double int_es;
   double diff_e;
   double prev_e;
   double prev_int_e;
   double prev_es;
   double prev_int_es;

   BOOL Saturation;
   int  TpCounter;	//number of callback ticks to start regulator
   int  TpAcc;		//current number of counted ticks

   TIME_SOURCE_CTX_PTR    pTimeCtx;
   INPUT_SIGNAL_RECT_PTR  pInputCtx;
   FIRST_ORDER_MODEL_PTR  pModelCtx;

}REG_PID;



//search best TE based on Scanning period value
static STATUS RegPidSetTE(IN double Tp, OUT int* Te, OUT int* TpCounter )
{

  long Te_array[]= {TIME_1000MS,
		            TIME_500MS,
					TIME_100MS,
					TIME_20MS,
     				TIME_10MS };
  long Tp_long;
  int i = 0;


  if(0.0 >= Tp)
  {
	  return STATUS_INVALID_PARAMS;
  }

  if((NULL == Te) || (NULL == TpCounter))
  {
	  return STATUS_PTR_ERROR;
  }

  //in worst case we will return TE_10MS with properly adjusted TpCounter
  //we assume the Tp must be multiplication of 10ms period

  Tp_long = (long)(Tp*1000.0);

  for(i = 0; i<ARRAY_SIZE(Te_array); i++)
  {
    if(0 == (Tp_long%Te_array[i]))
    {
       switch(Te_array[i])
       {
          case TIME_1000MS:
    	      *Te = TE_1000MS;
    	      break;

    	   case TIME_500MS:
    		  *Te = TE_500MS;
	      	  break;

    	   case TIME_100MS:
    	      *Te = TE_100MS;
    		  break;

    	   case TIME_20MS:
    		  *Te = TE_20MS;
    		  break;

    	   case TIME_10MS:
    		  *Te = TE_10MS;
    		  break;

    	   default:
    		   return STATUS_FAILURE;

    	   }

       *TpCounter = Tp_long/Te_array[i];
       break;
    }
  }
  //printf("%d, %d\n",*Te,*TpCounter);
  return STATUS_SUCCESS;


// return TE;
}

STATUS RegPidInit(REG_PID_PTR* ppPid)
{
	TIME_EVENT Events;
	int BestTimeEvent = TE_10MS;

	if(NULL == ppPid)
	{
  	   return STATUS_PTR_ERROR;
	}

	*ppPid = (REG_PID_PTR)malloc(sizeof(REG_PID));

	if(NULL == *ppPid)
	{
	   return STATUS_PTR_ERROR;
	}

	(*ppPid)->Reg_On = TRUE;
	(*ppPid)->P_sel  = TRUE;
	(*ppPid)->I_sel  = TRUE;
	(*ppPid)->D_sel = FALSE;
	(*ppPid)->AntiWindup = INTEGRATOR_CLAMPING;
	(*ppPid)->kp = 0.5;
	(*ppPid)->Ti = 5.0;
	(*ppPid)->Td = 0.0;
	(*ppPid)->Tp = 0.5;
	(*ppPid)->Tt = 5.0;
	(*ppPid)->CS_min = 0.0;
	(*ppPid)->CS_max = 3.0;

	   //runtime
	(*ppPid)->P = 0.0;
	(*ppPid)->I = 0.0;
	(*ppPid)->D = 0.0;
	(*ppPid)->CS_raw = 0.0;
	(*ppPid)->CS = 0.0;

	(*ppPid)->e = 0.0;
	(*ppPid)->es = 0.0;
	(*ppPid)->int_e = 0.0;
	(*ppPid)->int_es = 0.0;
	(*ppPid)->diff_e = 0.0;
	(*ppPid)->prev_e = 0.0;
	(*ppPid)->prev_int_e = 0.0;
	(*ppPid)->prev_es = 0.0;
	(*ppPid)->prev_int_es = 0.0;
	(*ppPid)->TpAcc = 0;
	(*ppPid)->TpCounter = (int)DIV_ROUND_CLOSEST((*ppPid)->Tp, T10_MS);


	//Determinate best Time Event to register on
	RegPidSetTE((*ppPid)->Tp,&BestTimeEvent,&((*ppPid)->TpCounter));

	//Register events to time observe
	Events = (TE_BOT |
			  BestTimeEvent);

    //call register API
    CreateObserver((void*)(*ppPid), Events, RegPidRun);


	return STATUS_SUCCESS;

}

STATUS RegPidPostInit(IO REG_PID_PTR pPid,
		              IN TIME_SOURCE_CTX_PTR pTimeCtx,
					  IN INPUT_SIGNAL_RECT_PTR pInputRect,
					  IN FIRST_ORDER_MODEL_PTR pModel)
{
	if((NULL == pPid)||(NULL == pTimeCtx)||(NULL == pInputRect)||(NULL == pModel))
	{
	  return STATUS_PTR_ERROR;
	}

	pPid->pTimeCtx  = pTimeCtx;
	pPid->pInputCtx = pInputRect;
	pPid->pModelCtx = pModel;


	return STATUS_SUCCESS;

}

STATUS RegPidClose(REG_PID_PTR pPid)
{
	free(pPid);
	pPid = NULL;
	return STATUS_SUCCESS;
}

void   RegPidRun(void* pInstance, const TIME_EVENT Events)
{
	REG_PID_PTR pPid = NULL;

	if(NULL == pInstance)
	{
	   return;
	}

	pPid = (REG_PID*)pInstance;


    //regulator Off forward SP to CS
    if(FALSE == pPid->Reg_On)
   	{
   	   pPid->CS_raw = RectangleSignalGetValue(pPid->pInputCtx);
   	   pPid->CS     = RectangleSignalGetValue(pPid->pInputCtx);
   	   return;
   	}


    //scanning period accu inc.
    (pPid->TpAcc)++;

    //if  TpAcc is < TpCounter it means we dont have to calculate new CS
    if((pPid->TpAcc) < (pPid->TpCounter))
    {
    	//do nothing
    	return;
    }

    //It is time to calculate new CS

    //save values from previous iteration
    pPid->prev_e = pPid->e;
    pPid->prev_int_e = pPid->int_e;

    pPid->prev_es = pPid->es;
    pPid->prev_int_es = pPid->int_es;

    //error value
    pPid->e =  RectangleSignalGetValue(pPid->pInputCtx) - FirstOrderModelGetOutput(pPid->pModelCtx);
    pPid->es = pPid->CS - pPid->CS_raw;


    //error integrator value
    pPid->int_e  =  pPid->prev_int_e + (pPid->Tp/2.0)*(pPid->prev_e + pPid->e);
    pPid->int_es =  pPid->prev_int_es + (pPid->Tp/2.0)*(pPid->prev_es + pPid->es);

    if(INTEGRATOR_CLAMPING == pPid->AntiWindup)
    {
  	   if((TRUE == pPid->Saturation) &&
  		 (0.0<(pPid->e * pPid->CS_raw)))
	   {
  		  //Integrator is stopped (e=0 is send to integrator) when CV saturates,
  		  //and error and CV have the same sign
  		  pPid->int_e = pPid->prev_int_e;
       }

    }

    //error diff value
    pPid->diff_e = pPid->e - pPid->prev_e;


    //CS
	//P
	pPid->P = (pPid->P_sel)? ((pPid->kp) * (pPid->e)) : (double)0.0;

	//I
	//Anti-windup algorithm
	if(BACK_CALCULATION == pPid->AntiWindup)
	{
	   //Tracking anti-windup, back-calculation
	   //http://www.scs-europe.net/services/ecms2006/ecms2006%20pdf/107-ind.pdf
	   pPid->I = (pPid->I_sel)? (((1.0/pPid->Ti)*(pPid->int_e))+((1.0/pPid->Tt)*(pPid->int_es))) : (double)0.0;
	}
	else
	{
       pPid->I = (pPid->I_sel)? ((1.0/pPid->Ti)*(pPid->int_e)) : (double)0.0;
	}


	//D
	pPid->D = (pPid->D_sel)? ((pPid->Td)*pPid->diff_e): (double)0.0;

	//P+I+D
	pPid->CS_raw = (pPid->P + pPid->I + pPid->D);

	//Saturation
	if(pPid->CS_max < pPid->CS_raw )
	{
		pPid->CS = pPid->CS_max;
		pPid->Saturation = TRUE;
	}
	else if(pPid->CS_min > pPid->CS_raw )
	{
	    pPid->CS = pPid->CS_min;
	    pPid->Saturation = TRUE;
	}
	else
	{
		pPid->CS = pPid->CS_raw;
		pPid->Saturation = FALSE;
	}


}

double RegPidGetCS(IN REG_PID_PTR pPid)
{
   if(NULL == pPid)
   {
	   return 0.0;
   }

   return pPid->CS;

}

char* RegPidGetHeader(IN REG_PID_PTR pPid)
{
   if(NULL != pPid)
   {
      return "CS[-], CS_raw[-], P[-], I[-], D[-], Error[-]";
   }
   else
   {
	   return "!!!Error!!!";
   }

}

STATUS RegPidGetValues(IN REG_PID_PTR pPid, OUT REG_PID_LOG_VALUES* pValues)
{
	if((NULL == pPid)||(NULL == pValues))
	{
		return STATUS_PTR_ERROR;
	}

	pValues->P         = pPid->P;
	pValues->I         = pPid->I;
	pValues->D         = pPid->D;
	pValues->CS        = pPid->CS;
	pValues->CS_raw    = pPid->CS_raw;
	pValues->e         = pPid->e;
	return STATUS_SUCCESS;
}

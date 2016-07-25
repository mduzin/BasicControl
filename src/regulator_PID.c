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

#include "sim_env.h"

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

   TIME_SOURCE_CTX_PTR    pTimeCtx;
   FIRST_ORDER_MODEL_PTR  pModelCtx;

}REG_PID;

STATUS RegPidInit(REG_PID_PTR* ppPid)
{
	TIME_EVENT Events;

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
	(*ppPid)->P_sel = TRUE;
	(*ppPid)->I_sel = TRUE;
	(*ppPid)->D_sel = FALSE;
	(*ppPid)->AntiWindup = BACK_CALCULATION;
	(*ppPid)->kp = 0.75;
	(*ppPid)->Ti = 3.0;
	(*ppPid)->Td = 0.0;
	(*ppPid)->Tp = 1.0;
	(*ppPid)->Tt = 5.0;
	(*ppPid)->CS_min = -3.0;
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

	//Register events to time observe
	Events = (TE_BOT |
   			  TE_10MS);

    //call register API
    CreateObserver((void*)(*ppPid), Events, RegPidRun);


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


    if((NULL == pPid->pTimeCtx) || (NULL == pPid->pModelCtx))
    {
	   return;
    }

    //regulator Off forward SP to CS
    if(FALSE == pPid->Reg_On)
   	{
   	   pPid->CS_raw = 1.0; // get SP from input signal
   	   return;
   	}

    //save values from previous iteration
    pPid->prev_e = pPid->e;
    pPid->prev_int_e = pPid->int_e;

    pPid->prev_es = pPid->es;
    pPid->prev_int_es = pPid->int_es;

    //error value
    pPid->e =  0.0; //simulation->Runtime.akt_SP - model->Runtime.y;
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

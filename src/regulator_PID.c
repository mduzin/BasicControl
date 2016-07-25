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

//Jesli aktualna probka rowna sie maksymalnej ilosci probek to wyznaczamy nowy CS
#define CalculateNewControlSignal(CurrentSample,MaxSample) (0 == (CurrentSample)%(MaxSample))

STATUS regulator_init(IN INIT_PID_PARAM *init_values,
		              IN SIMULATION_PARAM *SimulationParams,
		              OUT PID_PARAM *regulator)
{
	STATUS status = STATUS_SUCCESS;

	//parametry regulatora

	regulator->P_sel            = init_values->P_sel;
	regulator->I_sel            = init_values->I_sel;
	regulator->D_sel            = init_values->D_sel;
	regulator->AntiWindupV1_sel = init_values->AntiWindupV1_sel;
	regulator->AntiWindupV2_sel = init_values->AntiWindupV2_sel;
	regulator->Tp               = init_values->Tp; 	//probkowanie regulatora [s]
	regulator->Ti               = init_values->Ti;	//czas zdwojenia [s]
	regulator->Td               = init_values->Td;	//stala rozniczkowania
	regulator->Tt               = init_values->Tt;	//anti-windup tracking time
	regulator->kp               = init_values->kp;	//wzmocnienie regulatora

	//ograniczenie wyjscia regulatora
	regulator->CS_max           = init_values->CS_max;
	regulator->CS_min           = init_values->CS_min;

	//parametry dynamiczne
	regulator->Runtime.e             = 0.0;	 //	aktualny uchyb
	regulator->Runtime.es            = 0.0;
	regulator->Runtime.calka_e       = 0.0;
	regulator->Runtime.calka_es      = 0.0;
	regulator->Runtime.prev_e        = 0.0;
	regulator->Runtime.prev_es       = 0.0;
	regulator->Runtime.prev_calka_e  = 0.0;
	regulator->Runtime.prev_calka_es = 0.0;
	regulator->Runtime.rozniczka_e   = 0.0;

	//co ile iteracji wyznaczamy nowy Control Signal
	regulator->Runtime.il_pr_CS = DIV_ROUND_CLOSEST(regulator->Tp, SimulationParams->Tc);
	regulator->Runtime.zl_il_pr_CS= 0;

	//printf("il_CS: %ld\n\nd",regulator->Runtime.il_pr_CS);
	regulator->Initialized = FLG_SET;
	return status;
}

//funkcja obliczajaca wyjscie regulatora PID
STATUS regulator_run(SIMULATION_PARAM *simulation,PID_PARAM *regulator,MODEL_PARAM *model)
{

	if(FALSE == regulator->Pid_On)
	{
		regulator->Runtime.CS = simulation->Runtime.akt_SP;
		return STATUS_SUCCESS;
	}


    //sprawdzamy ktora mamy probke/iteracje symulacji
	if (CalculateNewControlSignal(simulation->Runtime.akt_pr_Tsym,regulator->Runtime.il_pr_CS))
	{
		//obliczmy nowy CS
        //przepisz poprzednie wartosci
		regulator->Runtime.prev_e = regulator->Runtime.e;
		regulator->Runtime.prev_calka_e = regulator->Runtime.calka_e;

		regulator->Runtime.prev_es = regulator->Runtime.es;
		regulator->Runtime.prev_calka_es = regulator->Runtime.calka_es;

		//oblicz uchyb
		regulator->Runtime.e = simulation->Runtime.akt_SP - model->Runtime.y;
		regulator->Runtime.es = regulator->Runtime.CS - regulator->Runtime.CS_raw;

		//oblicz calke

	    //Integrator clamping
	    //Integrator is stopped (e=0 is send to integrator) when CV saturates,
 	    //and error(uchyb) and CV have the same sign
		if((TRUE == regulator->AntiWindupV2_sel) &&
		   (FLG_SET == regulator->Saturation) &&
		   (0.0<(regulator->Runtime.e * regulator->Runtime.CS_raw)))
	    {
			regulator->Runtime.calka_e = regulator->Runtime.prev_calka_e;
	    }
		else
		{
			regulator->Runtime.calka_e  =  regulator->Runtime.prev_calka_e + (regulator->Tp/2.0)*(regulator->Runtime.prev_e + regulator->Runtime.e);
		}

		if(TRUE == regulator->AntiWindupV1_sel)
		{
			regulator->Runtime.calka_es =  regulator->Runtime.prev_calka_es + (regulator->Tp/2.0)*(regulator->Runtime.prev_es + regulator->Runtime.es);
		}



		//oblicz rozniczke
		regulator->Runtime.rozniczka_e = regulator->Runtime.e - regulator->Runtime.prev_e;


	 	//obliczanie CS
		//Czlon P
		regulator->Runtime.P = (regulator->P_sel)? ((regulator->kp) * (regulator->Runtime.e)) : (double)0.0;

		//Czlon I
		//Anti-windup algorithm
		if(TRUE == regulator->AntiWindupV1_sel)
		{
	       //Anti-windup algorithm
		   //Tracking anti-windup, back-calculation
		   //http://www.scs-europe.net/services/ecms2006/ecms2006%20pdf/107-ind.pdf
		   regulator->Runtime.I = (regulator->I_sel)? (((1.0/regulator->Ti)*(regulator->Runtime.calka_e))+((1.0/regulator->Tt)*(regulator->Runtime.calka_es))) : (double)0.0;

		}
		else
		{
			//Obliczanie cz�onu I zarowno dla wylaczonego AntiWindup'a
			//oraz windupa: Intergrator clamping (patrz sposob obliczania calki uchybu powyzej)
		    regulator->Runtime.I = (regulator->I_sel)? ((1.0/regulator->Ti)*(regulator->Runtime.calka_e)) : (double)0.0;
		}

		//Czlon D
		regulator->Runtime.D = (regulator->D_sel)? ((regulator->Td)*regulator->Runtime.rozniczka_e): (double)0.0;

		//P+I+D
		regulator->Runtime.CS_raw = (regulator->Runtime.P + regulator->Runtime.I + regulator->Runtime.D);

    	//Saturation
	    //nasycenie wyjscia regulatora
		regulator->Saturation = FLG_SET;
		if(regulator->CS_max < regulator->Runtime.CS_raw )
		{
			regulator->Runtime.CS = regulator->CS_max;
		}
		else if(regulator->CS_min > regulator->Runtime.CS_raw )
		{
		    regulator->Runtime.CS = regulator->CS_min;
		}
		else
		{
			regulator->Runtime.CS = regulator->Runtime.CS_raw;
			regulator->Saturation = FLG_CLRD;
		}

		//printf("Nowy CS:@ %f, CS: %3.3f, e: %3.3f calka_e: %3.3f\n",simulation->Runtime.akt_Tsym,regulator->Runtime.CS, regulator->Runtime.e,regulator->Runtime.calka_e);


	}

 return STATUS_SUCCESS;
}

STATUS regulator_close(PID_PARAM *regulator)
{
//narazie nie mamy nic do roboty przy zwijaniu regulatora
return STATUS_SUCCESS;
}



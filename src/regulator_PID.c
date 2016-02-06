/*
 * regulator_PI.c
 *
 *  Created on: Aug 8, 2015
 *      Author: mduzinki
 */

#include "common.h"
#include "regulator_PID.h"
#include "sim_env.h"
#include "model_1.h"

static STATUS regulator_antiwindup(IO PID_PARAM *regulator);

STATUS regulator_init(IN INIT_PID_PARAM *init_values,
		              IN SIMULATION_PARAM *SimulationParams,
		              OUT PID_PARAM *regulator)
{
	STATUS status = STATUS_SUCCESS;

	//parametry regulatora
	regulator->Pid_On = init_values->Pid_On; //wl/wyl algorytm regualacji
	regulator->P_sel = init_values->P_sel;
	regulator->I_sel = init_values->I_sel;
	regulator->D_sel = init_values->D_sel;
	regulator->AntiWindup_sel = init_values->AntiWindup_sel;
	regulator->Tp     = init_values->Tp; 	//probkowanie regulatora [s]
	regulator->Ti     = init_values->Ti;	//czas zdwojenia [s]
	regulator->Td     = init_values->Td;	//stala rozniczkowania
	regulator->Tt     = init_values->Tt;	//anti-windup tracking time
	regulator->kp     = init_values->kp;	//wzmocnienie regulatora

	//ograniczenie wyjscia regulatora
	regulator->CS_max = init_values->CS_max;
	regulator->CS_min = init_values->CS_min;

	//parametry dynamiczne
	regulator->Runtime.e = 0.0;			 //	aktualny uchyb
	regulator->Runtime.es = 0.0;
	regulator->Runtime.calka_e = 0.0;
	regulator->Runtime.calka_es = 0.0;
	regulator->Runtime.prev_e = 0.0;
	regulator->Runtime.prev_es = 0.0;
	regulator->Runtime.prev_calka_e = 0.0;
	regulator->Runtime.prev_calka_es = 0.0;
	regulator->Runtime.rozniczka_e = 0.0;

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

	}
	else
	{

		//sprawdzamy ktora mamy probke/iteracje symulacji
			if (0 == (simulation->Runtime.akt_pr_Tsym)%(regulator->Runtime.il_pr_CS))
			{
				//obliczmy nowy CS

				//przepisz poprzednie wartosci
				regulator->Runtime.prev_e = regulator->Runtime.e;
				regulator->Runtime.prev_calka_e = regulator->Runtime.calka_e;

				//oblicz uchyb
				regulator->Runtime.e = simulation->Runtime.akt_SP - model->Runtime.y;
				//oblicz calke
				regulator->Runtime.calka_e =  regulator->Runtime.prev_calka_e + (regulator->Tp/2.0)*(regulator->Runtime.prev_e + regulator->Runtime.e);
				//oblicz rozniczke
				regulator->Runtime.rozniczka_e = regulator->Runtime.e - regulator->Runtime.prev_e;



				//obliczanie CS
				regulator->Runtime.P = (regulator->P_sel)? ((regulator->kp) * (regulator->Runtime.e)) : (double)0.0;
				regulator->Runtime.I = (regulator->I_sel)? ((1.0/regulator->Ti)*(regulator->Runtime.calka_e)) : (double)0.0;
				regulator->Runtime.D = (regulator->D_sel)? ((regulator->Td)*regulator->Runtime.rozniczka_e): (double)0.0;
				regulator->Runtime.CS = (regulator->Runtime.P + regulator->Runtime.I + regulator->Runtime.D);

				//Placeholder for Anti-windup algorithm
				if(TRUE == regulator->AntiWindup_sel)
				{
				   //Anti-windup algorithm
					regulator_antiwindup(regulator);

				}


				//Saturation
				//nasycenie wyjscia regulatora
				if(regulator->CS_max < regulator->Runtime.CS )
				{
					regulator->Runtime.CS = regulator->CS_max;
				}
				else if(regulator->CS_min > regulator->Runtime.CS )
				{
				    regulator->Runtime.CS = regulator->CS_min;
				}


				//printf("Nowy CS:@ %f, CS: %3.3f, e: %3.3f calka_e: %3.3f\n",simulation->Runtime.akt_Tsym,regulator->Runtime.CS, regulator->Runtime.e,regulator->Runtime.calka_e);


	    }

	}


 return STATUS_SUCCESS;
}

STATUS regulator_close(PID_PARAM *regulator)
{
//narazie nie mamy nic do roboty przy zwijaniu regulatora
return STATUS_SUCCESS;
}

static STATUS regulator_antiwindup(IO PID_PARAM *regulator)
{
  //Algorithm Tracking anti-windup, back-calculation
  //Algorytm anti-windup modyfikuje nam sposob obliczania calki uchybu
  //Ref: http://www.scs-europe.net/services/ecms2006/ecms2006%20pdf/107-ind.pdf
  //wpierw obliczamy uchyb dla czlona calkujacego: czyli (1/Ti)*e
  // potem liczymy uchyb wyjscia regulatora (roznica miedzy wyj. reg. a limitem regulatora)
  // i calosc symujemy i robimy na tym nowa calke
  // potem przeliczamy sterowanie na nowo

  regulator->Runtime.es = 0.0;

  if(NULL == regulator)
  {
 	 return STATUS_PTR_ERROR;
  }
  else
  {

     if(regulator->CS_max < regulator->Runtime.CS )
     {
	   regulator->Runtime.es = regulator->Runtime.CS - regulator->CS_max;
     }
     else if(regulator->CS_min > regulator->Runtime.CS )
     {
	   regulator->Runtime.es =  regulator->Runtime.CS - regulator->CS_min;
     }
     //obliczamy calke sygnalu es
     regulator->Runtime.calka_es =  regulator->Runtime.prev_calka_es + (regulator->Tp/2.0)*(regulator->Runtime.prev_es + regulator->Runtime.es);

     //modyfikujemy tor I regulatora
     regulator->Runtime.I = (regulator->I_sel)? (((1.0/regulator->Ti)*(regulator->Runtime.calka_e))+((1.0/regulator->Tt)*(regulator->Runtime.calka_es))) : (double)0.0;
     //i ponowine obliczamy CS
     regulator->Runtime.CS = (regulator->Runtime.P + regulator->Runtime.I + regulator->Runtime.D);

     return STATUS_SUCCESS;
  }
}



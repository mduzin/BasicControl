/*
 * sim_env.c

 *
 *  Created on: Aug 10, 2015
 *      Author: mduzinki
 */
#include "common.h"
#include "sim_env.h"


//inicjalizacja srodowiska symulacji
STATUS simulation_init(IN INIT_SIMULATION_PARAM *init_values, OUT SIMULATION_PARAM *simulation)
{
	simulation->Tc = init_values->Tc;
	simulation->Tsym = init_values->Tsym;
	simulation->il_pr_Tsym = (simulation->Tsym)/(simulation->Tc);
	simulation->Runtime.akt_Tsym = 0;
	simulation->Runtime.akt_pr_Tsym = 0;
	//simulation->Input_signal.SP = Step_signal;

	simulation->Initialized = FLG_SET;
	return STATUS_SUCCESS;
}

STATUS simulation_close(SIMULATION_PARAM *simulation)
{
//nothing to do
	simulation->Stop = FLG_SET;
	return STATUS_SUCCESS;
}

STATUS simulation_iter(SIMULATION_PARAM *simulation)
{
	if(FLG_SET == simulation->Initialized)
	{

		if(simulation->il_pr_Tsym >= simulation->Runtime.akt_pr_Tsym)
		{
			simulation->Run = FLG_SET;
			simulation->Runtime.akt_pr_Tsym++;
			simulation->Runtime.akt_Tsym = simulation->Runtime.akt_Tsym + simulation->Tc;
			return STATUS_SUCCESS;
		}
		else
		{
			simulation->Run = FLG_CLRD;
			simulation->Stop = FLG_SET;
			return STATUS_FAILURE;
		}

	}
	else
	{
		printf("Srodowisko symulacji niezainicjalizowane.\n");
		return STATUS_FAILURE;
	}

}


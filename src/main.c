/*
 * main.c
 *
 *  Created on: Jul 8, 2015
 *      Author: mduzinki
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "common.h"
#include "file_lib.h"
#include "sim_env.h"
#include "regulator_PID.h"
#include "model_1.h"
#include "test_env.h"

//<TODO:> Porobic makra ktore autamtycznie enable/disableuja poszczegolne model+regulator
/*#define PURE_MODEL_ENABLE
#define MODEL_ENABLE
#define MODEL_WINDUP_ENABLE*/

//Zmienne globalne
SIMULATION_PARAM SimulationParams;

MODEL_PARAM ModelParams;
PID_PARAM   RegulatorPID;
LOG_FILE_PARAM ModelTest_log;

MODEL_PARAM ModelWindupParams;
PID_PARAM   RegulatorWindupPID;
LOG_FILE_PARAM ModelWindupTest_log;

MODEL_PARAM ModelPureParams;
PID_PARAM RegulatorPurePID;
LOG_FILE_PARAM ModelPureTest_log;



int main(int argc, char *argv[])
{

 INIT_TEST_VALUES init_values;
 INIT_TEST_VALUES init_windup_values;
 INIT_TEST_VALUES init_pure;


 init_values.sim.Tc = 0.01;
 init_values.sim.Tsym = 400.0;

 init_values.model.k= 1.0;
 init_values.model.Ts = 4.0;
 init_values.model.Tdelay = 0.5;

 init_values.pid.Pid_On = TRUE;
 init_values.pid.P_sel = TRUE;
 init_values.pid.I_sel = TRUE;
 init_values.pid.D_sel = FALSE;
 init_values.pid.AntiWindup_sel = FALSE;
 init_values.pid.kp = 0.25;
 init_values.pid.Ti = 3.0;
 init_values.pid.Td = 0.0;
 init_values.pid.Tp = 1.0;
 init_values.pid.Tt = 2.0;
 init_values.pid.CS_min = -3.0;
 init_values.pid.CS_max = 3.0;

 init_values.log_file_name = "Model1.csv";

 //Windup model ma wlaczona opcje windupa
 init_windup_values = init_values;
 init_windup_values.pid.AntiWindup_sel = TRUE;

 init_values.log_file_name = "Model1W.csv";


 //Pure ma tylko wylaczonego PID'a reszta jest ta sama
 init_pure = init_values;
 init_pure.pid.Pid_On = FALSE;

 init_values.log_file_name = "Model1P.csv";


	printf("Symulacja obiektu I-ego rzedu\n");

	//inicjalizacja
	log_file_init("Model1.csv",&ModelTest_file);
	log_file_init("Model1W.csv",&ModelWindupTest_file);

	sim_func.Init(&init_values.sim,&SimulationParams);
	//Nasz regulator + obiekt
	model_func.Init(&init_values.model,&SimulationParams,&ModelParams);
	regulator_func.Init(&init_values.pid,&SimulationParams,&RegulatorPID);

	//Nasz regulator z windupem + obiekt
	model_func.Init(&init_windup_values.model,&SimulationParams,&ModelWindupParams);
	regulator_func.Init(&init_windup_values.pid,&SimulationParams,&RegulatorWindupPID);

	//Tylko obiekt
	model_func.Init(&init_pure.model,&SimulationParams,&ModelPureParams);
	regulator_func.Init(&init_pure.pid,&SimulationParams,&RegulatorPurePID);

	//petla iteracji symulacji obiektu
	while(TRUE)
	{
		//wywolaj równania sygnalu wejsciowego, regulator i modelu obiektu
		 sim_func.Input(&SimulationParams);
		 //obiekt+wlaczony regulator
		 regulator_func.Run(&SimulationParams,&RegulatorPID,&ModelParams);
		 model_func.Run(&SimulationParams,&RegulatorPID,&ModelParams);
		 //obiekt+wlaczony regulator z windupem
		 regulator_func.Run(&SimulationParams,&RegulatorWindupPID,&ModelWindupParams);
		 model_func.Run(&SimulationParams,&RegulatorWindupPID,&ModelWindupParams);
		 //obiekt+wylaczony regulator
		 regulator_func.Run(&SimulationParams,&RegulatorPurePID,&ModelPureParams);
		 model_func.Run(&SimulationParams,&RegulatorPurePID,&ModelPureParams);

		 //zapis do pliku wynikow kroku symulacji
		 log_file_write(ModelTest_file,&SimulationParams,&RegulatorPID,&ModelParams);
		 log_file_write(ModelWindupTest_file,&SimulationParams,&RegulatorWindupPID,&ModelWindupParams);

		 //aktualizacja parametrow symulacji, sprawdz czy koniec symulacji
		 if(STATUS_SUCCESS != (sim_func.Iter(&SimulationParams)))
		    break;

	}


	//koniec symulacji - zwijamy sie

	model_func.Close(&ModelParams);
	model_func.Close(&ModelPureParams);
	model_func.Close(&ModelWindupParams);
	regulator_func.Close(&RegulatorPID);
	regulator_func.Close(&RegulatorPurePID);
	regulator_func.Close(&RegulatorWindupPID);
	sim_func.Close(&SimulationParams);
	log_file_close(ModelTest_file);
	log_file_close(ModelWindupTest_file);
	return 0;
}


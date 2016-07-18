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
#include "input_signal.h"
#include "time_source.h"

//<TODO:> Porobic makra ktore autamtycznie enable/disableuja poszczegolne model+regulator
/*#define PURE_MODEL_ENABLE
#define MODEL_ENABLE
#define MODEL_WINDUP_ENABLE*/

//Zmienne globalne
SIMULATION_PARAM SimulationParams;

MODEL_PARAM ModelParams;
PID_PARAM   RegulatorPID;
LOG_PARAM   ModelTest_log;

MODEL_PARAM ModelWindupParams;
PID_PARAM   RegulatorWindupPID;
LOG_PARAM   ModelWindupTest_log;

MODEL_PARAM ModelWindup2Params;
PID_PARAM   RegulatorWindup2PID;
LOG_PARAM   ModelWindup2Test_log;

MODEL_PARAM ModelPureParams;
PID_PARAM   RegulatorPurePID;
LOG_PARAM   ModelPureTest_log;


INPUT_SIGNAL_RECT InputRect;

//Funcje sygnalow wejsciowych sa wywolywane w kazdej iteracji (czyli co okres calkowania)
//skok jednostkowy
STATUS step_signal(SIMULATION_PARAM *simulation)
{
    simulation->Runtime.akt_SP = 1.0;
	return STATUS_SUCCESS;
}

//sygnal prostokatny
STATUS rectangle_signal(SIMULATION_PARAM *simulation)
{
	double period = 25.0;  		 //czas co jaki zmieniamy stan wyjscia
	static double acc_period = 0.0; //zliczony czas trawania stanu
    static int state = 0;
    double output;


    if(0.0 != simulation->Runtime.akt_Tsym)
   	 {
   		 //dzialamy jest czas symulacji jest wiekszy od 0.0

   		 //zliczony czas period jest wiekszy-rowny od trwania period = zmiana stanu
   		 if(acc_period >= period)
   		 {

   			 //zmiana stanu
   			 if(++state > 2)
   			 {
   				//po stanie 4 wracamy do stanu 0
   				 state = 0;
   			 }

   			 //byla zmiana stanu wiec wyzeruj zliczony period(acc_period)
   			 acc_period = 0.0;
   		 }
   		 else
   		 {
   			 //nie ma zmiany stanu tylko zwieksz acc_period
   			 acc_period += simulation->Tc;
   		 }
   	 }
   	 else
   	 {
   		//inicjalizacja maszynki (simulation->Runtime.akt_Tsym == 0)
   		 state = 0;
   	 }
    switch(state)
    	 {
    	 case 0:
    		 output = 4.0;
    		 break;
    	 case 1:
    		 output = -4.0;
    		 break;
    	 default:
    		 output = 0.0;
    		 break;
    	 }

    	//wystaw do globalnej zmiennej symulacji
    	simulation->Runtime.akt_SP = output;


	return STATUS_SUCCESS;
}

//sygnal pila
STATUS saw_signal(SIMULATION_PARAM *simulation)
{
	 double period = 25.0;  		 //czas co jaki zmieniamy stan wyjscia
	 static double acc_period = 0.0; //zliczony czas trawania stanu
	 static int state  = 0;			 //stan maszynki 0,1,2,3
	 double output;					 //zwaracana wartosc



	 if(0.0 != simulation->Runtime.akt_Tsym)
	 {
		 //dzialamy jest czas symulacji jest wiekszy od 0.0

		 //zliczony czas period jest wiekszy-rowny od trwania period = zmiana stanu
		 if(acc_period >= period)
		 {

			 //zmiana stanu
			 if(++state > 4)
			 {
				//po stanie 4 wracamy do stanu 0
				 state = 0;
			 }

			 //byla zmiana stanu wiec wyzeruj zliczony period(acc_period)
			 acc_period = 0.0;
		 }
		 else
		 {
			 //nie ma zmiany stanu tylko zwieksz acc_period
			 acc_period += simulation->Tc;
		 }
	 }
	 else
	 {
		//inicjalizacja maszynki (simulation->Runtime.akt_Tsym == 0)
		 state = 0;
	 }

	 //ustaw wyjscie w zalezenosci od stanu
	 switch(state)
	 {
	 case 0:
	 case 2:
		 output = 0.0;
		 break;
	 case 1:
		 output = 1.0;
		 break;
	 case 3:
		 output = -1.0;
 		 break;
	 default:
		 output = 0.0;
		 break;
	 }

	//wystaw do globalnej zmiennej symulacji
	simulation->Runtime.akt_SP = output;
    return STATUS_SUCCESS;

}




int main(int argc, char *argv[])
{

 INIT_TEST_VALUES init_values;
 INIT_TEST_VALUES init_windup_values;
 INIT_TEST_VALUES init_windup2_values;
 INIT_TEST_VALUES init_pure;


 init_values.sim.Tc = 0.01;
 init_values.sim.Tsym = 200.0;

 init_values.model.k= 1.0;
 init_values.model.Ts = 4.0;
 init_values.model.Tdelay = 0.5;

 init_values.pid.Pid_On = TRUE;
 init_values.pid.P_sel = TRUE;
 init_values.pid.I_sel = TRUE;
 init_values.pid.D_sel = FALSE;
 init_values.pid.AntiWindupV1_sel = FALSE;
 init_values.pid.AntiWindupV2_sel = FALSE;
 init_values.pid.kp = 0.75;
 init_values.pid.Ti = 3.0;
 init_values.pid.Td = 0.0;
 init_values.pid.Tp = 1.0;
 init_values.pid.Tt = 5.0;
 init_values.pid.CS_min = -3.0;
 init_values.pid.CS_max = 3.0;

 init_values.log.filename = "Model1.csv";

 //Windup model ma wlaczona opcje windupa
 init_windup_values = init_values;
 init_windup_values.pid.AntiWindupV1_sel = TRUE;

 init_windup_values.log.filename = "Model1W1.csv";

 //Windup2 model ma wlaczona opcje windupa
 init_windup2_values = init_values;
 init_windup2_values.pid.AntiWindupV2_sel = TRUE;

 init_windup2_values.log.filename = "Model1W2.csv";

 //Pure ma tylko wylaczonego PID'a reszta jest ta sama
 init_pure = init_values;
 init_pure.pid.Pid_On = FALSE;

 init_pure.log.filename = "Model1P.csv";


	printf("Symulacja obiektu I-ego rzedu\n");

	//inicjalizacja
	log_func.Init(init_values.log,&ModelTest_log);
	log_func.Init(init_windup_values.log,&ModelWindupTest_log);
	log_func.Init(init_windup2_values.log,&ModelWindup2Test_log);
	log_func.Init(init_pure.log,&ModelPureTest_log);


	sim_func.Init(&init_values.sim,&SimulationParams);
	//Nasz regulator + obiekt
	model_func.Init(&init_values.model,&SimulationParams,&ModelParams);
	regulator_func.Init(&init_values.pid,&SimulationParams,&RegulatorPID);

	//Nasz regulator z windupem + obiekt
	model_func.Init(&init_windup_values.model,&SimulationParams,&ModelWindupParams);
	regulator_func.Init(&init_windup_values.pid,&SimulationParams,&RegulatorWindupPID);

	//Nasz regulator z windupem2 + obiekt
	model_func.Init(&init_windup2_values.model,&SimulationParams,&ModelWindup2Params);
	regulator_func.Init(&init_windup2_values.pid,&SimulationParams,&RegulatorWindup2PID);

	//Tylko obiekt
	model_func.Init(&init_pure.model,&SimulationParams,&ModelPureParams);
	regulator_func.Init(&init_pure.pid,&SimulationParams,&RegulatorPurePID);

	//petla iteracji symulacji obiektu
	/*while(TRUE)
	{
		//wywolaj równania sygnalu wejsciowego, regulator i modelu obiektu
		 sim_func.Input(&SimulationParams);
		 //obiekt+wlaczony regulator
		 regulator_func.Run(&SimulationParams,&RegulatorPID,&ModelParams);
		 model_func.Run(&SimulationParams,&RegulatorPID,&ModelParams);
		 //obiekt+wlaczony regulator z windupem
		 regulator_func.Run(&SimulationParams,&RegulatorWindupPID,&ModelWindupParams);
		 model_func.Run(&SimulationParams,&RegulatorWindupPID,&ModelWindupParams);
		 //obiekt+wlaczony regulator z windupem2
		 regulator_func.Run(&SimulationParams,&RegulatorWindup2PID,&ModelWindup2Params);
		 model_func.Run(&SimulationParams,&RegulatorWindup2PID,&ModelWindup2Params);
		 //obiekt+wylaczony regulator
		 regulator_func.Run(&SimulationParams,&RegulatorPurePID,&ModelPureParams);
		 model_func.Run(&SimulationParams,&RegulatorPurePID,&ModelPureParams);

		 //zapis do pliku wynikow kroku symulacji
		 log_func.Write(&ModelTest_log,&SimulationParams,&RegulatorPID,&ModelParams);
		 log_func.Write(&ModelWindupTest_log,&SimulationParams,&RegulatorWindupPID,&ModelWindupParams);
		 log_func.Write(&ModelWindup2Test_log,&SimulationParams,&RegulatorWindup2PID,&ModelWindup2Params);
		 log_func.Write(&ModelPureTest_log,&SimulationParams,&RegulatorPurePID,&ModelPureParams);

		 //aktualizacja parametrow symulacji, sprawdz czy koniec symulacji
		 if(STATUS_SUCCESS != (sim_func.Iter(&SimulationParams)))
		    break;

	}*/

	TimeSourceInit();
	RectangleSignalInit(&InputRect);
	TimeSourceTick();



	//koniec symulacji - zwijamy sie

	model_func.Close(&ModelParams);
	model_func.Close(&ModelPureParams);
	model_func.Close(&ModelWindupParams);
	model_func.Close(&ModelWindup2Params);
	regulator_func.Close(&RegulatorPID);
	regulator_func.Close(&RegulatorPurePID);
	regulator_func.Close(&RegulatorWindupPID);
	regulator_func.Close(&RegulatorWindup2PID);
	sim_func.Close(&SimulationParams);
	log_func.Close(&ModelTest_log);
	log_func.Close(&ModelWindupTest_log);
	log_func.Close(&ModelWindup2Test_log);
	log_func.Close(&ModelPureTest_log);
	return 0;
}


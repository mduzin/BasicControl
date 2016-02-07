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


//Zmienne globalne
SIMULATION_PARAM SimulationParams;

MODEL_PARAM ModelParams;
PID_PARAM RegulatorPID;

MODEL_PARAM ModelWindupParams;
PID_PARAM RegulatorWindupPID;

MODEL_PARAM ModelPureParams;
PID_PARAM RegulatorPurePID;

FILE *input_file;
FILE *output_file;

FILE *ModelTest_file;
FILE *ModelWindupTest_file;

STATUS get_command_line_arg(IN int argc,IN char *argv[],OUT char** input_filename, OUT char** output_filename);


int main(int argc, char *argv[])
{
 char *input_filename;
 char *output_filename;

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
 init_values.pid.kp = 0.75;
 init_values.pid.Ti = 3.0;
 init_values.pid.Td = 0.0;
 init_values.pid.Tp = 1.0;
 init_values.pid.Tt = 2.0;
 init_values.pid.CS_min = -3.0;
 init_values.pid.CS_max = 3.0;

 //Referencyjny ma tylko wylaczonego PID'a i inny model
 init_windup_values = init_values;
 init_windup_values.pid.AntiWindup_sel = TRUE;


 //Pure ma tylko wylaczonego PID'a reszta jest ta sama
 init_pure = init_values;
 init_pure.pid.Pid_On = FALSE;


	printf("Symulacja obiektu I-ego rzedu\n");
	get_command_line_arg(argc,argv,&input_filename,&output_filename);

	printf("Symulacja obiektu I-ego rzedu\n");
	printf("Plik wejsciowy: %s\n",input_filename);
	printf("Plik wyjsciowy: %s\n",output_filename);

	//inicjalizacja
	output_file_init(output_filename,&output_file);
	log_file_init("ModelTestLog.csv",&ModelTest_file);
	log_file_init("ModelWindupTestLog.csv",&ModelWindupTest_file);

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
		 output_file_write(output_file,
				           SimulationParams.Runtime.akt_Tsym,
				           SimulationParams.Runtime.akt_SP,
				           ModelParams.Runtime.y,
				           ModelPureParams.Runtime.y,
				           ModelWindupParams.Runtime.y,
				           RegulatorPID.Runtime.CS
				           );

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
	output_file_close(output_file);
	log_file_close(ModelTest_file);
	log_file_close(ModelWindupTest_file);
	return 0;
}


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
    double time_on = 50.0;
    double time_off = 50.0;
    static double state = 1.0;
    static double acc_time = 0.0;

    if(0.0 != simulation->Runtime.akt_Tsym)
    {
    	if(1.0 == state)
    	{
    		if(acc_time >= time_on)
    		{
    			state = 0.5;
    			acc_time = 0.0;
 				goto exit;
    		}
    	}

    	if(0.5 == state)
    	{
    		if(acc_time >= time_off)
    		{
    		  state = 1.0;
    		  acc_time = 0.0;
    		  goto exit;
    		 }
    	}
    }

exit:
simulation->Runtime.akt_SP = (double)state * 4;
acc_time += simulation->Tc;

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


STATUS get_command_line_arg(IN int argc,IN char *argv[],OUT char** input_filename, OUT char** output_filename)
{
 STATUS Status = STATUS_SUCCESS;
 //char buff[MAX_STR_LEN];
 int input_strlen;
 int output_strlen;
 //sprawdz czy ilosc parametrow sie zgadza
 if(3 != argc)
 {
	 printf("Niepoprawna ilosc argumentow\n");
	 return STATUS_FAILURE;
 }
 else
 {
     input_strlen = strlen(argv[1])+1;
     output_strlen = strlen(argv[2])+1;

     *input_filename = (char*)malloc(input_strlen * sizeof(char));
     *output_filename = (char*)malloc(output_strlen * sizeof(char));

     strcpy(*input_filename, argv[1]);
     strcpy(*output_filename, argv[2]);

 }


 return Status;

}


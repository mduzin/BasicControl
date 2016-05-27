/*
 * file_lib.c
 *
 *  Created on: Aug 8, 2015
 *      Author: mduzinki
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "file_lib.h"


STATUS log_init(IN INIT_LOG_PARAM init_param,
		        IO LOG_PARAM *log)
{
	char buff[MAX_STR_LEN];

	//sprawdzamy poprawnosc parametrów wejsciowych

	//NULL pointery
	if((NULL == init_param.filename)|| (NULL == log))
	{
		return STATUS_PTR_ERROR;
	}

	//poprawnosc nazwy pliku
	if((MAX_STR_LEN <= strlen(init_param.filename)) || \
	   (0 == strlen(init_param.filename)))
	{
		return STATUS_FAILURE;
	}

	log->file = fopen(init_param.filename,"w");
	if(NULL != log->file)
	{
		printf("Otworto plik z logiem: %s.\n",init_param.filename);
		//tworzymy nag³owki kolumn
		sprintf(buff, "Czas[s],SetPoint[-],Wyjscie[-],Sterowanie[-],Uchyb[-],CalkaUchyb[-],RozniczkaUchyb[-],P[-],I[-],D[-],es[-],Calka_es[-]\n");
		fputs(buff, log->file);
		return STATUS_SUCCESS;
	}
   return STATUS_FAILURE;
}


STATUS log_write(IO LOG_PARAM *log,
		         IN SIMULATION_PARAM *simulation,
				 IN PID_PARAM *regulator,
				 IN MODEL_PARAM *model)
{
	 char buff[MAX_STR_LEN];

	 if((NULL == log) || (NULL == log->file))
	 {
		 return STATUS_PTR_ERROR;
	 }
	 else
     {
		 sprintf(buff, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n",
				 (simulation->Runtime.akt_Tsym),
				 (simulation->Runtime.akt_SP),
				 (model->Runtime.y),
				 (regulator->Runtime.CS),
				 (regulator->Runtime.e),
				 (regulator->Runtime.calka_e),
				 (regulator->Runtime.rozniczka_e),
				 (regulator->Runtime.P),
				 (regulator->Runtime.I),
				 (regulator->Runtime.D),
				 (regulator->Runtime.es),
				 (regulator->Runtime.calka_es)
		       );
		 fputs(buff, log->file);
		 //remove \0 z konca linii
		 return STATUS_SUCCESS;
	  }

	 return STATUS_FAILURE;
}

STATUS log_close(LOG_PARAM *log)
{

	if((NULL == log) || (NULL == log->file))
	{
		return STATUS_PTR_ERROR;
	}

	if(!fclose(log->file))
	{
		printf("Zamknieto plik loga.\n");
		return STATUS_SUCCESS;
	}
	else
	{
		printf("Nie udalo sie zamknac pliku log.\n");
		return STATUS_FAILURE;
	}
}

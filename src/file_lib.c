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

STATUS output_file_init(IN char* filename, IO FILE **output_file)
{

	char buff[MAX_STR_LEN];

	//sprawdzamy poprawnosc parametrów wejsciowych

	//NULL pointery
	if((NULL == filename)|| (NULL == output_file))
	{
		return STATUS_PTR_ERROR;
	}

	//poprawnosc nazwy pliku
	if((MAX_STR_LEN <= strlen(filename)) || (0 == strlen(filename)))
	{
		return STATUS_FAILURE;
	}

	*output_file = fopen(filename,"w");
	if(NULL != *output_file)
	{
		printf("Otworto plik wynikowy...\n");
		//tworzymy nag³owki kolumn
		sprintf(buff, "Czas[s], SetPoint[-], Wyjscie[-],Wyjscie Pure[-], Wyjscie Ref[-], Sterowanie[-] \n");
		fputs(buff, *output_file);
		return STATUS_SUCCESS;
	}
	else
	{
		printf("Nie udalo sie otworzyc pliku wynikowego...\n");
		return STATUS_FAILURE;
	}

}

STATUS output_file_close(FILE *output_file)
{

	if(NULL == output_file)
	{
		return STATUS_PTR_ERROR;
	}

	if(!fclose(output_file))
	{
		printf("Zamknieto plik wynikowy.\n");
		return STATUS_SUCCESS;
	}
	else
	{
		printf("Nie udalo sie zamknac pliku wynikowego.\n");
		return STATUS_FAILURE;
	}
}



STATUS output_file_write(FILE *output_file,
		double Tsym,
		double SP,
		double output,
		double outputPure,
		double outputRef,
		double CS)
{
 char buff[MAX_STR_LEN];

 if(output_file)
  {
	 sprintf(buff, "%.4f,%.3f,%.3f,%.3f,%.3f,%2.3f\n",Tsym,SP,output,outputPure,outputRef,CS);
	 fputs(buff, output_file);
	 //remove \0 z konca linii
	 return STATUS_SUCCESS;
  }

return STATUS_FAILURE;

}

STATUS log_file_init(IN char* filename, IO FILE **log_file)
{
	char buff[MAX_STR_LEN];

	//sprawdzamy poprawnosc parametrów wejsciowych

	//NULL pointery
	if((NULL == filename)|| (NULL == log_file))
	{
		return STATUS_PTR_ERROR;
	}

	//poprawnosc nazwy pliku
	if((MAX_STR_LEN <= strlen(filename)) || (0 == strlen(filename)))
	{
		return STATUS_FAILURE;
	}

	*log_file = fopen(filename,"w");
	if(NULL != *log_file)
	{
		printf("Otworto plik z logiem: %s.\n",filename);
		//tworzymy nag³owki kolumn
		sprintf(buff, "Czas[s], SetPoint[-],Wyjscie[-],Sterowanie[-],Uchyb[-],CalkaUchyb[-],RozniczkaUchyb[-]\n");
		fputs(buff, *log_file);
		return STATUS_SUCCESS;
	}
}


STATUS log_file_write(IO FILE *output_file,
		              IN SIMULATION_PARAM *simulation,
					  IN PID_PARAM *regulator,
					  IN MODEL_PARAM *model)
{
	 char buff[MAX_STR_LEN];

	 if(output_file)
	  {
		 sprintf(buff, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n",
				 (simulation->Runtime.akt_Tsym),
				 (simulation->Runtime.akt_SP),
				 (model->Runtime.y),
				 (regulator->Runtime.CS),
				 (regulator->Runtime.e),
				 (regulator->Runtime.calka_e),
				 (regulator->Runtime.rozniczka_e));
		 fputs(buff, output_file);
		 //remove \0 z konca linii
		 return STATUS_SUCCESS;
	  }

	 return STATUS_FAILURE;
}

STATUS log_file_close(FILE *log_file)
{

	if(NULL == log_file)
	{
		return STATUS_PTR_ERROR;
	}

	if(!fclose(log_file))
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

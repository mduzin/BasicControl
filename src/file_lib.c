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



STATUS write_output_line(FILE *output_file,
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

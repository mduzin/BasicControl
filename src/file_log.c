/*
 * file_log.c
 *
 *  Created on: 19 lip 2016
 *      Author: M
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "file_log.h"
#include "model_1.h"
#include "time_source.h"
#include "time_observer.h"
#include "regulator_PID.h"

typedef struct _FILE_LOG
{
	char* filename;
	FILE *file;

	TIME_SOURCE_CTX_PTR    pTimeCtx;
    INPUT_SIGNAL_RECT_PTR  pInputCtx;
	FIRST_ORDER_MODEL_PTR  pModelCtx;
	REG_PID_PTR            pPidCtx;

}FILE_LOG;

typedef struct _LOG_LINE
{
	TIME_SOURCE_LOG_VALUES       CurrTime;
	INPUT_SIGNAL_RECT_LOG_VALUES InputSignal;
	REG_PID_LOG_VALUES           Pid;
	FIRST_ORDER_MODEL_LOG_VALUES Model;

}LOG_LINE;

STATUS FileLogInit(FILE_LOG_PTR* ppFile)
{
	TIME_EVENT Events;

	if(NULL == ppFile)
	{
	   return STATUS_PTR_ERROR;
	}

	*ppFile = (FILE_LOG_PTR)malloc(sizeof(FILE_LOG));

	if(NULL == *ppFile)
	{
	   return STATUS_PTR_ERROR;
	}

	(*ppFile)->filename = "log1.csv";

	//lenght of file name illegals
	if((MAX_STR_LEN <= strlen((*ppFile)->filename)) || \
	   (0 == strlen((*ppFile)->filename)))
	{
		return STATUS_FAILURE;
	}

	//Register events to time observe
	Events = (TE_BOT |
			  TE_100MS);

    //call register API
    CreateObserver((void*)(*ppFile), Events, FileLogWrite);


	return STATUS_SUCCESS;
}

STATUS FileLogPostInit(IO FILE_LOG_PTR pFile,
					   IN TIME_SOURCE_CTX_PTR pTimeCtx,
					   IN INPUT_SIGNAL_RECT_PTR pInputRect,
					   IN FIRST_ORDER_MODEL_PTR pModel,
					   IN REG_PID_PTR pPid)
{

	char LogHeader[MAX_STR_LEN];

	if(NULL == pFile)
	{
   	   return STATUS_PTR_ERROR;
	}

	if((NULL == pTimeCtx)||(NULL == pInputRect)||(NULL == pModel)||(NULL == pPid))
	{
	  return STATUS_PTR_ERROR;
	}

	pFile->file = fopen(pFile->filename,"w");
	if(NULL == pFile->file)
	{
       return STATUS_FAILURE;
	}

	pFile->pTimeCtx  = pTimeCtx;
	pFile->pInputCtx = pInputRect;
	pFile->pModelCtx = pModel;
	pFile->pPidCtx   = pPid;


	printf("Otworto plik z logiem: %s.\n",pFile->filename);



	strcat(LogHeader,TimeSourceGetHeader(pTimeCtx));
	strcat(LogHeader,", ");
	strcat(LogHeader,RectangleSignalGetHeader(pInputRect));
	strcat(LogHeader,", ");
	strcat(LogHeader,FirstOrderModelGetHeader(pModel));
	strcat(LogHeader,", ");
	strcat(LogHeader,RegPidGetHeader(pPid));
	strcat(LogHeader,"\n");
	//in case of too long string
	LogHeader[MAX_STR_LEN-1]= '\0';

	fputs(LogHeader, pFile->file);

	//printf("Header: %s\n",LogHeader);

	return STATUS_SUCCESS;


}

STATUS FileLogClose(FILE_LOG_PTR pFile)
{
	if(NULL == pFile)
	{
		return STATUS_PTR_ERROR;
	}

    if(OK == fclose(pFile->file))
	{
		printf("Zamknieto plik loga: %s.\n", pFile->filename);
		return STATUS_SUCCESS;
	}
	else
	{
		printf("Nie udalo sie zamknac pliku log.\n");
		return STATUS_FAILURE;
	}


}

void   FileLogWrite(void* pInstance, const TIME_EVENT Events)
{
	LOG_LINE LogLine;
	char buff[MAX_STR_LEN];
	FILE_LOG* pFile = NULL;

	if(NULL == pInstance)
	{
		return;
	}

	pFile = (FILE_LOG*)pInstance;

	//Gather logged values
	//<TODO:> check status codes
	TimeSourceGetLogValues(pFile->pTimeCtx, &LogLine.CurrTime);
	RectangleSignalGetValues(pFile->pInputCtx, &LogLine.InputSignal);
	FirstOrderModelGetValues(pFile->pModelCtx, &LogLine.Model);
	RegPidGetValues(pFile->pPidCtx, &LogLine.Pid);

	//prepare log line
	//save log line
	sprintf(buff, "%lu,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n",
					 ((long)LogLine.CurrTime),
					 (LogLine.InputSignal),
					 (LogLine.Model.u),
					 (LogLine.Model.y),
					 (LogLine.Model.y_delayed),
					 (LogLine.Pid.CS),
					 (LogLine.Pid.CS_raw),
					 (LogLine.Pid.P),
					 (LogLine.Pid.I),
					 (LogLine.Pid.D),
  				     (LogLine.Pid.e)
			       );
			 fputs(buff, pFile->file);

	return;
}

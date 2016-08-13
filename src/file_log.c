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

	//<TODO:>fetch header info from handles

	return STATUS_SUCCESS;


}

STATUS FileLogClose(FIRST_ORDER_MODEL_PTR pFile)
{
	return STATUS_SUCCESS;
}
void   FileLogWrite(void* pInstance, const TIME_EVENT Events)
{
	return;
}

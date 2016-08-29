/*
 * file_log.h
 *
 *  Created on: 19 lip 2016
 *      Author: M
 */

#ifndef FILE_LOG_H_
#define FILE_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "adt.h"

#include "time_source.h"
#include "input_signal.h"
#include "model_1.h"
#include "regulator_PID.h"

//ADT
//moved to adt.h

//API for ADT

STATUS FileLogInit(FILE_LOG_PTR*);
STATUS FileLogPostInit(IO FILE_LOG_PTR pFile,
					   IN TIME_SOURCE_CTX_PTR pTimeCtx,
					   IN INPUT_SIGNAL_RECT_PTR pInputRect,
					   IN FIRST_ORDER_MODEL_PTR pModel,
					   IN REG_PID_PTR pPid);

STATUS FileLogClose(FILE_LOG_PTR);
void   FileLogWrite(void* pInstance, const TIME_EVENT Events);


#endif /* FILE_LOG_H_ */

/*
 * model_1.h
 *
 *  Created on: Aug 9, 2015
 *      Author: mduzinki
 */

#ifndef MODEL_1_H_
#define MODEL_1_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "adt.h"
#include "time_source.h"
#include "regulator_PID.h"


//API for ADT
STATUS FirstOrderModelInit(FIRST_ORDER_MODEL_PTR*);
STATUS FirstOrderModelPostInit(IO FIRST_ORDER_MODEL_PTR pModel,
							   IN REG_PID_PTR pPid,
							   IN TIME_SOURCE_CTX_PTR pTimeCtx);
STATUS FirstOrderModelClose(FIRST_ORDER_MODEL_PTR);
void FirstOrderModelRun(void* pInstance, const TIME_EVENT Events);

double FirstOrderModelGetOutput(FIRST_ORDER_MODEL_PTR);



#endif /* MODEL_1_H_ */

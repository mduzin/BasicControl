/*
 * regulator_PI.h
 *
 *  Created on: Aug 8, 2015
 *      Author: mduzinki
 */

#ifndef REGULATOR_PI_H_
#define REGULATOR_PI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "adt.h"
#include "time_source.h"
#include "input_signal.h"
#include "model_1.h"

//shared structure
typedef struct _REG_PID_LOG_VALUES
{
   double P;		//Proportional value
   double I;		//Integrator value
   double D;		//Differential value
   double CS_raw;	//Raw Control Signal before saturation limit check etc.
   double CS;	    //Output Control Signal
   double e;        //error
}REG_PID_LOG_VALUES;

typedef enum _ANTI_WINDUP_TYPE
{
	NO_ANTI_WINDUP = 0,
	BACK_CALCULATION,		//Tracking anti-windup, back-calculation
	INTEGRATOR_CLAMPING     //Integrator clamping
}ANTI_WINDUP_TYPE;


//API for ADT
STATUS RegPidInit(REG_PID_PTR*);
STATUS RegPidPostInit(IO REG_PID_PTR pPid,
		              IN TIME_SOURCE_CTX_PTR pTimeCtx,
					  IN INPUT_SIGNAL_RECT_PTR pInputRect,
					  IN FIRST_ORDER_MODEL_PTR pModel);
STATUS RegPidClose(REG_PID_PTR);
void   RegPidRun(void* pInstance, const TIME_EVENT Events);

double RegPidGetCS(REG_PID_PTR);

char* RegPidGetHeader(IN REG_PID_PTR);
STATUS RegPidGetValues(IN REG_PID_PTR pPid, OUT REG_PID_LOG_VALUES* pValues);




#endif /* REGULATOR_PI_H_ */

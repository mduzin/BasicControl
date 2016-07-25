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

//ADT
typedef struct _REG_PID* REG_PID_PTR;

//API for ADT
STATUS RegPidInit(REG_PID_PTR*);
STATUS RegPidClose(REG_PID_PTR);
void   RegPidRun(void* pInstance, const TIME_EVENT Events);

typedef enum _ANTI_WINDUP_TYPE
{
	NO_ANTI_WINDUP = 0,
	BACK_CALCULATION,		//Tracking anti-windup, back-calculation
	INTEGRATOR_CLAMPING            //Integrator clamping
}ANTI_WINDUP_TYPE;


#endif /* REGULATOR_PI_H_ */

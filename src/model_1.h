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
#include "sim_env.h"

//ADT
typedef struct _FIRST_ORDER_MODEL* FIRST_ORDER_MODEL_PTR;

//API for ADT
STATUS FirstOrderModelInit(FIRST_ORDER_MODEL_PTR*);
STATUS FirstOrderModelClose(FIRST_ORDER_MODEL_PTR);
void FirstOrderModelRun(void* pInstance, const TIME_EVENT Events);



#endif /* MODEL_1_H_ */

/*
 * main.c
 *
 *  Created on: Jul 8, 2015
 *      Author: mduzinki
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "common.h"
#include "adt.h"
#include "regulator_PID.h"
#include "model_1.h"
#include "input_signal.h"
#include "time_source.h"

TIME_SOURCE_CTX_PTR   pTimeCtx;
INPUT_SIGNAL_RECT_PTR pInputRect;
FIRST_ORDER_MODEL_PTR pModel;
REG_PID_PTR           pPid;



int main(int argc, char *argv[])
{
	printf("Symulacja obiektu I-ego rzedu\n");

	//First phase init - allocate memory and set variables
	TimeSourceInit(&pTimeCtx);
	RectangleSignalInit(&pInputRect);
	FirstOrderModelInit(&pModel);
	RegPidInit(&pPid);


	//Second phase init - share handles, avoid cross dependency
    FirstOrderModelPostInit(pModel,pPid,pTimeCtx);
    RegPidPostInit(pPid,pTimeCtx,pInputRect,pModel);

	//run simulation
	TimeSourceTick(pTimeCtx);

	//finishing simulation - free memory
	FirstOrderModelClose(pModel);
	//RegPidClose(pPid);

	printf("Zakonczono symulacje.\n");
	return 0;
}


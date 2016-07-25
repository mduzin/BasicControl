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
#include "regulator_PID.h"
#include "model_1.h"
#include "input_signal.h"
#include "time_source.h"

INPUT_SIGNAL_RECT InputRect;

TIME_SOURCE_CTX_PTR pTimeCtx;


int main(int argc, char *argv[])
{
	printf("Symulacja obiektu I-ego rzedu\n");

	TimeSourceInit(&pTimeCtx);
	RectangleSignalInit(&InputRect);
	TimeSourceTick(pTimeCtx);

	return 0;
}


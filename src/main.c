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

TIME_SOURCE_CTX_PTR   pTimeCtx;
INPUT_SIGNAL_RECT_PTR pInputRect;



int main(int argc, char *argv[])
{
	printf("Symulacja obiektu I-ego rzedu\n");

	TimeSourceInit(&pTimeCtx);
	RectangleSignalInit(&pInputRect);
	TimeSourceTick(pTimeCtx);

	return 0;
}


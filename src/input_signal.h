/*
 * input_signal.h
 *
 *  Created on: 17 lip 2016
 *      Author: M
 */

#ifndef INPUT_SIGNAL_H_
#define INPUT_SIGNAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "common.h"
#include "adt.h"
#include "time_observer.h"


STATUS StepSignalInit(IN INPUT_SIGNAL_STEP_PTR* ppInput);
void   StepSignal(void* pInstance, const TIME_EVENT Events);
double StepSignalGetValue(IN INPUT_SIGNAL_STEP_PTR pInput);

STATUS RectangleSignalInit(IN INPUT_SIGNAL_RECT_PTR* ppInput);
void   RectangleSignal(void* pInstance, const TIME_EVENT Events);
double RectangleSignalGetValue(IN INPUT_SIGNAL_RECT_PTR pInput);
char*  RectangleSignalGetHeader(IN INPUT_SIGNAL_RECT_PTR pInput);


#endif /* INPUT_SIGNAL_H_ */

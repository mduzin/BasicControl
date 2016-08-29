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

typedef double INPUT_SIGNAL_RECT_LOG_VALUES;

STATUS StepSignalInit(IN INPUT_SIGNAL_STEP_PTR* ppInput);
void   StepSignal(void* pInstance, const TIME_EVENT Events);
double StepSignalGetValue(IN INPUT_SIGNAL_STEP_PTR pInput);

STATUS RectangleSignalInit(IN INPUT_SIGNAL_RECT_PTR* ppInput);
void   RectangleSignal(void* pInstance, const TIME_EVENT Events);
double RectangleSignalGetValue(IN INPUT_SIGNAL_RECT_PTR pInput);

char*  RectangleSignalGetHeader(IN INPUT_SIGNAL_RECT_PTR pInput);
STATUS RectangleSignalGetValues(IN INPUT_SIGNAL_RECT_PTR pInput, OUT INPUT_SIGNAL_RECT_LOG_VALUES* pValues);


#endif /* INPUT_SIGNAL_H_ */

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
#include "time_observer.h"

//<TODO:> Do przerobienia na ADT
typedef struct _INPUT_SIGNAL
{
   double Value;				//aktualna wartosc sygnalu

} INPUT_SIGNAL;

STATUS InputSignalInit(IN INPUT_SIGNAL* pInput);
void rect_singal(void* pInstance, const TIME_EVENT Events);


#endif /* INPUT_SIGNAL_H_ */

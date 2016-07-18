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
typedef struct _INPUT_SIGNAL_RECT
{
   double Value;				//aktualna wartosc sygnalu
   double InitValue;			//Wartosc poczatkowa sygnalu
   int    SwitchTime;			//czas ktory zmieniamy sygnal

   int InternalCounter;			//zmienna wewn.

} INPUT_SIGNAL_RECT;

STATUS RectangleSignalInit(IN INPUT_SIGNAL_RECT* pInput);
void RectangleSignal(void* pInstance, const TIME_EVENT Events);


#endif /* INPUT_SIGNAL_H_ */

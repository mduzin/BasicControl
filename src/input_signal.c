/*
 * input_signal.c
 *
 *  Created on: 26 maj 2016
 *      Author: M
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "common.h"
#include "time_observer.h"

typedef struct _INPUT_SIGNAL
{
   double Value;				//aktualna wartosc sygnalu

} INPUT_SIGNAL;

//callbacki
void rect_singal(void* pInstance, const TIME_EVENT Events)
{
	INPUT_SIGNAL* pInput = NULL;

	if(NULL == pInstance)
	{
		return;
	}

	pInput = (INPUT_SIGNAL*)pInstance;

	if(IS_IDX_SET_IN_MAP(TE_500MS_IDX,Events))
	{
		pInput->Value = (pInput->Value) * (-1.0);
	}
}

//globale
INPUT_SIGNAL Input;


//tworzenie obiektu Input Signal
STATUS InputSignalInit(IN INPUT_SIGNAL* pInput)
{
	STATUS Status = STATUS_SUCCESS;
	TIME_EVENT Events;

	//init internal values
	pInput->Value = 0.0;

	//Register for time observer
	Events = (TE_20MS |
			  TE_500MS);
	//call register API
	CreateObserver((void*)pInput, Events, rect_singal);

	return Status;

}


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
#include "input_signal.h"
#include "time_observer.h"


//callbacki
void rect_singal(void* pInstance, const TIME_EVENT Events)
{
	INPUT_SIGNAL* pInput = NULL;

	if(NULL == pInstance)
	{
		return;
	}

	pInput = (INPUT_SIGNAL*)pInstance;

	if(TRUE == (IS_IDX_SET_IN_MAP(TE_20MS_IDX,Events)))
	{
	   printf("Event 20ms\n");
	}

	if(TRUE == (IS_IDX_SET_IN_MAP(TE_500MS_IDX,Events)))
	{
		printf("Event 500ms\n");
		pInput->Value = (pInput->Value) * (-1.0);
	}
}


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


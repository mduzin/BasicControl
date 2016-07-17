/*
 * time_observer.c
 *
 *  Created on: 13 lip 2016
 *      Author: M
 */

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "time_observer.h"
#include "time_source.h"


typedef struct _TIME_OBSERVER
{
   void*                   Instance;
   TIME_EVENT              Event;
   TIME_EVENT_NOTIFICATION Callback;
} TIME_OBSERVER;

//Funkcja zwroci adres z zallokowana w pamieci struktura,
//ktora nastepnie attachujemy do time_source
STATUS CreateObserver(IN void* pInstance,
		              IN TIME_EVENT Event,
					  IN TIME_EVENT_NOTIFICATION pCallback)
{
	TIME_OBSERVER_PTR pObserver;

	if((NULL == pCallback) || (NULL == pInstance))
	{
		return STATUS_PTR_ERROR;
	}

	//zallokuj pamiec dla struktury obserwatora
	pObserver = (TIME_OBSERVER_PTR)malloc(sizeof(TIME_OBSERVER));

	if(NULL == (pObserver))
	{
		return STATUS_PTR_ERROR;
	}

	(pObserver)->Instance = pInstance;
	(pObserver)->Callback = pCallback;
	(pObserver)->Event = Event;
	//dodaj do time_source'a
	attach(pObserver);
	return STATUS_SUCCESS;
}

void* ObserverGetInstance(IN TIME_OBSERVER_PTR Observer)
{
   return Observer->Instance;
}

TIME_EVENT ObserverGetEvent(IN TIME_OBSERVER_PTR Observer)
{
   return Observer->Event;
}
TIME_EVENT_NOTIFICATION ObserverGetCallback(IN TIME_OBSERVER_PTR Observer)
{
   return Observer->Callback;
}

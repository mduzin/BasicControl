/*
 * time_observer.c
 *
 *  Created on: 13 lip 2016
 *      Author: M
 */

#include "common.h"
#include "time_observer.h"


typedef struct _TIME_OBSERVER
{
   void*                   Instance;
   TIME_EVENT              Event;
   TIME_EVENT_NOTIFICATION Callback;
} TIME_OBSERVER;

//Funkcja zwroci adres z zallokowana w pamieci struktura,
//ktora nastepnie attachujemy do time_source
STATUS CreateObserver(OUT TIME_OBSERVER_PTR Observer,
		              IN TIME_EVENT Events,
					  IN TIME_EVENT_NOTIFICATION Callback)
{
	return STATUS_SUCCESS;
}

TIME_EVENT ObserverGetEvent(IN TIME_OBSERVER_PTR Observer)
{
   return Observer->Event;
}
TIME_EVENT_NOTIFICATION ObserverGetCallback(IN TIME_OBSERVER_PTR Observer)
{
   return Observer->Callback;
}

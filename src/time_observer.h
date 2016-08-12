/*
 * time_observer.h
 *
 *  Created on: 20 cze 2016
 *      Author: M
 */

#ifndef TIME_OBSERVER_H_
#define TIME_OBSERVER_H_

#include "common.h"
#include "adt.h"

//Typ wskaznika na funkcje callback wywolana, gdy wydarzy sie jakis time event okreslony w event_id
typedef void (*TIME_EVENT_NOTIFICATION)(void* pInstance, const TIME_EVENT Event_id);


//deklaracje funkcji dla ADT obserwatora czasu
STATUS CreateObserver(IN void* pInstance,
		              IN TIME_EVENT Event,
					  IN TIME_EVENT_NOTIFICATION Callback);

//deklaracje funkcji dla ADT obserwatora czasu
STATUS DestroyObserver(IN TIME_EVENT Event,
					   IN TIME_EVENT_NOTIFICATION Callback);

void*                   ObserverGetInstance(IN TIME_OBSERVER_PTR Observer);
TIME_EVENT              ObserverGetEvent(IN TIME_OBSERVER_PTR);
TIME_EVENT_NOTIFICATION ObserverGetCallback(IN TIME_OBSERVER_PTR);







#endif /* TIME_OBSERVER_H_ */

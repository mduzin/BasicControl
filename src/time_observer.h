/*
 * time_observer.h
 *
 *  Created on: 20 cze 2016
 *      Author: M
 */

#ifndef TIME_OBSERVER_H_
#define TIME_OBSERVER_H_

//Typ wskaznika na funkcje callback wywolana, gdy wydarzy sie jakis time event okreslony w event_id
typedef void (*TIME_EVENT_NOTIFICATION)(const TIME_EVENT Event_id);

//ADT dla typu obserwator czasu
typedef struct _TIME_OBSERVER* TIME_OBSERVER_PTR;

//deklaracje funkcji dla ADT obserwatora czasu
//funkcja zwroci adres z zallokowana w pamieci struktura,
//ktora nastepnie attachujemy do time_source
STATUS CreateObserver(OUT TIME_OBSERVER_PTR,
		              IN TIME_EVENT Events,
					  IN TIME_EVENT_NOTIFICATION Callback);

TIME_EVENT              ObserverGetEvent(IN TIME_OBSERVER_PTR);
TIME_EVENT_NOTIFICATION ObserverGetCallback(IN TIME_OBSERVER_PTR);







#endif /* TIME_OBSERVER_H_ */

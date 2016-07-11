/*
 * time_observer.h
 *
 *  Created on: 20 cze 2016
 *      Author: M
 */

#ifndef TIME_OBSERVER_H_
#define TIME_OBSERVER_H_

//Typ wskaznika na funkcje callback wywolana, gdy wydarzy sie jakis time event okreslony w event_id
typedef void (*TimeEventNotification)(void* Instance, const UINT8* Event_id);

typedef struct _TIME_OBSERVER
{
   void*                 Instance;
   UINT8                 Event;
   TimeEventNotification notification_callback;
} TIME_OBSERVER;

#endif /* TIME_OBSERVER_H_ */

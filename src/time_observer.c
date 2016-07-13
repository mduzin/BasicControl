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
   TIME_EVENT_NOTIFICATION notification_callback;
} TIME_OBSERVER;

TIME_EVENT ObserverGetEvent(IN TIME_OBSERVER_PTR Observer)
{
   return Observer->Event;
}
TIME_EVENT_NOTIFICATION ObserverGetCallback(IN TIME_OBSERVER_PTR Observer)
{
   return Observer->notification_callback;
}

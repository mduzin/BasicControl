/*
 * time_source.h
 *
 *  Created on: 20 cze 2016
 *      Author: M
 */

#ifndef TIME_SOURCE_H_
#define TIME_SOURCE_H_

#include "common.h"
#include "time_observer.h"

//Dodanie do listy nastepnego obserwatora
STATUS attach(IN TIME_OBSERVER_PTR Observer);
//Usuniecie z listy obserwatora
STATUS detach(IN TIME_OBSERVER_PTR Observer);

//tikniecie zegara
void TimeTick(void);

#endif /* TIME_SOURCE_H_ */

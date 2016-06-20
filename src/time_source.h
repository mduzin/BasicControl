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

/*time events*/
//TE_BOT     - Time Event: Begin of time
//TE_10MS    - Time Event: 10ms time tick
//TE_20MS    - Time Event: 20ms time tick
//TE_100MS   - Time Event: 100ms time tick
//TE_500MS   - Time Event: 500ms time tick
//TE_1000MS  - Time Event: 1000ms time tick
//TE_EOT     - Time Event: End of time
#define TE_BOT    (1 << 0)
#define TE_10MS   (1 << 1)
#define TE_20MS   (1 << 2)
#define TE_100MS  (1 << 3)
#define TE_500MS  (1 << 4)
#define TE_1000MS (1 << 5)
#define TE_EOT    (1 << 6)

#define NO_EVENT   0

//Dodanie do listy nastepnego obserwatora
STATUS attach(IN TIME_OBSERVER* Observer, UINT8 Event_id);

//Usuniecie z listy obserwatora
STATUS detach(IN TIME_OBSERVER* Observer);

<TODO:>
//STATUS modify(IN TimeObserver* observer,UINT8 event_id);

#endif /* TIME_SOURCE_H_ */

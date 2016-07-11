/*
 * time_source.c
 *
 *  Created on: 20 cze 2016
 *      Author: M
 */

#include "time_source.h"

struct _NODE {
  TIME_OBSERVER Item;
  struct _NODE *Next;
} NODE;

struct _LINKED_LIST
{
NODE Head;
NODE Tail;
//itp..
}LINKED_LIST;



//Dodanie do listy nastepnego obserwatora
STATUS attach(IN TIME_OBSERVER* Observer, UINT8 Event_id);
STATUS detach(IN TIME_OBSERVER* Observer);
static void TimeTick(void);
static LINKED_LIST Observers;		//Lista z handles do poszczegolnych obserwatorow

//minimalny tick naszego zegara
//w zaleznosci od implementacji moze to byc licznik
// moze to byc select z timeoutem
// moze to byc sleep itp..
static void TimeTick(void)
{
 UINT8 Events = NO_EVENT;		    //zmienna lokalna z wszystkimi time eventami jakie wystapily w tym wywolaniu
 UINT8 EventsFiltered = NO_EVENT;   //time eventy zmaskowane z observatorem ktorego bedziemy notyfikowac (filtrujemy po to zeby nie informawac o eventach na ktore observawtor sie nie rejestrowal)
 struct LINKED_LIST* Node = Observers.Head;

 //logika tick'a zegar

 //na podstawie internal danych w zmiennej Event zapalamy bity, jaki event wystapil
 //zeby wiedziec ktorych obserwatorow notyfikowac
 //Events = xxx | yyy | zzz;


 while(NULL != Node) {
    TIME_OBSERVER* Observer = &Node->Item;
    EventsFiltered = Events & Observer->Event;
    if(NO_EVENT != EventsFiltered)
    {
       Observer->notification_callback(Observer->Instance, EventsFiltered);
    }
    Node = Node->Next;
 }



}

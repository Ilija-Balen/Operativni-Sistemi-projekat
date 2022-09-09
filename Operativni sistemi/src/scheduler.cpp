//
// Created by marko on 20.4.22..
//

#include "../h/scheduler.hpp"

mojaLista<CCB> Scheduler::readyCoroutineQueue;

CCB *Scheduler::get()
{
    return readyCoroutineQueue.izvadi();
}

void Scheduler::put(CCB *ccb)
{
    readyCoroutineQueue.dodaj(ccb);
}
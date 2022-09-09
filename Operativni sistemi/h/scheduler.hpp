//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP

#include "mojaLista.h"

class CCB;

class Scheduler
{
private:
    static mojaLista<CCB> readyCoroutineQueue;

public:
    static CCB *get();

    static void put(CCB *ccb);
    void* operator new (size_t size){
        return MemoryAllocator::mem_alloc(size);
    }
    void operator delete(void* ptr){
        MemoryAllocator::mem_free(ptr);
    }
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP

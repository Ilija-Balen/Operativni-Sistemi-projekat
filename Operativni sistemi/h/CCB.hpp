//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP


#include "scheduler.hpp"
#include "syscall_c.hpp"
#include "MemoryAllocator.hpp"
#include "../h/riscv.hpp"
// Coroutine Control Block
class CCB
{
public:
    ~CCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    void setBlocked(bool val){blocked=val;}

    bool isBlocked()const {return blocked;}

    void setDeleted(bool val){deleted=val;}

    bool isDeleted()const {return deleted;}

    void *getArgFje()const {return argumentiFje;}

    using Body = void (*)(void*);

    static CCB *createCoroutine(Body body);

    static void yield();

    static CCB *running;

    void t_start();
    void* operator new (size_t size);
    void operator delete(void* ptr);
    void *operator new[](size_t n);
    void operator delete[](void *p);
private:
    friend class Riscv;
    friend class Thread;


     CCB(thread_t*handle, void (*start_routine)(void *),void *arg)
     {
        *handle=this;
        body=start_routine;
        argumentiFje=arg;
        finished= false;
        blocked=false;
        deleted=false;
        //stack=((start_routine != nullptr) ? (uint64*)MemoryAllocator::mem_alloc(STACK_SIZE) : 0);
         stack=(uint64*)MemoryAllocator::mem_alloc(STACK_SIZE);

        //context.ra=((start_routine != nullptr) ? (uint64)&threadWrapper : 0);
        context.ra=(uint64)&threadWrapper;
        context.sp= (uint64)&stack[STACK_SIZE/sizeof(uint64)];
        //context.sp= ((start_routine != nullptr )? (uint64)&stack[STACK_SIZE/sizeof(uint64)] : 0);


    }

    CCB(Body _body){
        finished=false;
        blocked=false;
        deleted=false;
        body = _body;
        stack=((_body != nullptr) ? (uint64*)MemoryAllocator::mem_alloc(STACK_SIZE) : 0);
        context.ra=((_body != nullptr) ? (uint64)&threadWrapper : 0);
        context.sp= ((_body != nullptr )? (uint64)&stack[STACK_SIZE/sizeof(uint64)] : 0);
     }
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };
    void*pok;
     void *argumentiFje;
    Body body;
    uint64 *stack;
    Context context;
    bool finished;
    static void threadWrapper();
    static void contextSwitch(Context *oldContext, Context *runningContext);
    bool blocked;
    bool deleted;
    static void dispatch();

    static uint64 constexpr STACK_SIZE = 4096;
};



#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP

//
// Created by marko on 20.4.22..
//

#include "../h/CCB.hpp"


extern "C" void pushR();
extern "C" void popR();

CCB *CCB::running = nullptr;

CCB *CCB::createCoroutine(Body body)
{
    return new CCB(body);
}

void CCB::yield()
{
    pushR();

    CCB::dispatch();

    popR();
}

void CCB::dispatch()
{
    CCB *old = running;
    if (!old->isFinished()&& !old->isBlocked()) { Scheduler::put(old); }
    running = Scheduler::get();

    CCB::contextSwitch(&old->context, &running->context);
}

void CCB::t_start() {
    if (body != nullptr)  Scheduler::put(this);

}

void *CCB::operator new(size_t size) {
    return MemoryAllocator::mem_alloc(size);
}

void CCB::operator delete(void *ptr) {
    MemoryAllocator::mem_free(ptr);
}

void CCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->argumentiFje);
    thread_exit();

}

void *CCB::operator new[](size_t n) {
    return MemoryAllocator::mem_alloc(n);
}

void CCB::operator delete[](void *p) {
    MemoryAllocator::mem_free(p);
}

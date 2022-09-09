//
// Created by os on 8/15/22.
//

#include "../h/_sem.hpp"
#include "../h/printing.hpp"

void *_sem::operator new(size_t size) {
    return MemoryAllocator::mem_alloc(size);
}

void _sem::operator delete(void *ptr) {
    MemoryAllocator::mem_free(ptr);
}

void _sem::s_wait() {
    --init;
    if(init<0){
        blokiraneNiti.dodaj(CCB::running);
        CCB::running->setBlocked(true);
        CCB::yield();
    }

}

void _sem::s_signal() {
    ++init;
    if(init<=0){
        CCB*p=blokiraneNiti.izvadi();
        p->setBlocked(false);
        Scheduler::put(p);
    }
}

void _sem::s_close() {
    bool flag=false;
    do{
        CCB*p=blokiraneNiti.izvadi();

        if(p!= nullptr){
            p->setBlocked(false);
            p->setDeleted(true);
            Scheduler::put(p);
            flag= true;

        }else{
            flag=false;
        }
    }while(flag);
   //delete this;
}

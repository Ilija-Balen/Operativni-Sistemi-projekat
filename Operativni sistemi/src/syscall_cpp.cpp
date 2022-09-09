//
// Created by os on 8/5/22.
//

#include "../h/syscall_cpp.hpp"

Thread::Thread(void (*body)(void *), void *arg) {
    thread_create1(&myHandle,body,arg);
}

int Thread::start() {
    thread_start(myHandle);
    return 0;
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread() {
    thread_create1(&myHandle, Thread::ThreadWrapper, this);
}

void Thread::ThreadWrapper(void *nit) {
    Thread* n=(Thread*)nit;
    n->run();

}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

int Semaphore::wait() {
    sem_wait(myHandle);
    return 0;
}

int Semaphore::signal() {
    sem_signal(myHandle);
    return 0;
}

char Console::getc(){
	return ::getc();
}

void Console::putc(char c){
	::putc(c);
}

void* operator new (size_t size){
    return mem_alloc(size);
}
void operator delete (void*ptr){
    mem_free(ptr);
}

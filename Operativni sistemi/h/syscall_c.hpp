//
// Created by os on 8/5/22.
//

#ifndef PROJECT_BASE_SYSCALL_C_HPP
#define PROJECT_BASE_SYSCALL_C_HPP
#include "../lib/hw.h"
//#include "CCB.hpp"

void* mem_alloc (size_t size);
int mem_free (void*p);
class CCB;
typedef CCB* thread_t;

int thread_create(
        thread_t*handle,
        void(*start_routine)(void*),
        void *arg
        );

int thread_create1(thread_t*handle,
                   void(*start_routine)(void*),
                   void *arg);

int thread_exit();

void thread_dispatch();
void thread_start(thread_t handle);

class _sem;
typedef _sem* sem_t;

int sem_open(
        sem_t* handle,
        unsigned init
        );

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal(sem_t id);

typedef unsigned long time_t;
int time_sleep (time_t time);

const int EOF=-1;
char getc();
void putc(char c);

void user_mode();
void system_mode();
#endif //PROJECT_BASE_SYSCALL_C_HPP

//
// Created by os on 8/5/22.
//
#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../h/printing.hpp"



int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {

    //prosledis argumente sistemskog poziva u registre i zoves ecall, zatim u handleru se pravi objekat CCB
    __asm__ volatile("mv a6,%[arg2]"::[arg2] "r"(arg));
    __asm__ volatile("mv a2,%[arg1]"::[arg1] "r"(start_routine));
    __asm__ volatile("mv a1,%[arg0]"::[arg0] "r"(handle));


    uint64 brSisPoziva=0x11;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");
    int a;
    __asm__ volatile ("mv %[ptr], a5":[ptr]"=r"(a));
    if(a==0)return 0;
    return -11;
}

void thread_start(thread_t handle) {
    __asm__ volatile("mv a1,%[arg0]"::[arg0] "r"(handle));
    uint64 brSisPoziva=0x14;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");
}

int thread_exit(){
    uint64 brSisPoziva=0x12;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");
    int a;
    __asm__ volatile ("mv %[ptr], a5":[ptr]"=r"(a));
    if(a==0)return 0;
    return -12;

}
void thread_dispatch(){
    uint64 brSisPoziva=0x13;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");
}


void* mem_alloc (size_t size){
    __asm__ volatile("mv a1,%[arg0]"::[arg0] "r"(size));
    uint64 brSisPoziva=0x01;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");
    void *ptr;
    __asm__ volatile ("mv %[ptr], a0":[ptr]"=r"(ptr));
    return ptr;
}
int mem_free (void*p ){
    __asm__ volatile("mv a1,%[pointer]"::[pointer] "r"(p));
    uint64 brSisPoziva=0x02;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));

    __asm__ volatile("ecall");
    return 0;
}

int sem_open(sem_t *handle, unsigned int init) {
    __asm__ volatile("mv a2,%[arg1]"::[arg1] "r"(init));
    __asm__ volatile("mv a1,%[arg0]"::[arg0] "r"(handle));

     uint64 brSisPoziva=0x21;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");

    int a;
    __asm__ volatile ("mv %[ptr], a5":[ptr]"=r"(a));
    if(a==0)return 0;
    return -21;
}

int sem_close(sem_t handle){
    __asm__ volatile("mv a1,%[arg0]"::[arg0] "r"(handle));
    uint64 brSisPoziva=0x22;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");
    int a;
    __asm__ volatile ("mv %[ptr], a5":[ptr]"=r"(a));
    if(a==0)return 0;
    return -22;
}

int sem_wait(sem_t id){
    __asm__ volatile("mv a1,%[arg0]"::[arg0] "r"(id));
    uint64 brSisPoziva=0x23;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");
    int a;
    __asm__ volatile ("mv %[ptr], a5":[ptr]"=r"(a));
    if(a==0)return 0;
    return -23;
}

int sem_signal(sem_t id){
    __asm__ volatile("mv a1,%[arg0]"::[arg0] "r"(id));
    uint64 brSisPoziva=0x24;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");
    int a;
    __asm__ volatile ("mv %[ptr], a5":[ptr]"=r"(a));
    if(a==0)return 0;
    return -24;
}

int thread_create1(thread_t *handle, void (*start_routine)(void *), void *arg) {
    __asm__ volatile("mv a6,%[arg2]"::[arg2] "r"(arg));
    __asm__ volatile("mv a2,%[arg1]"::[arg1] "r"(start_routine));
    __asm__ volatile("mv a1,%[arg0]"::[arg0] "r"(handle));


    uint64 brSisPoziva=0x15;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");
    return 0;
}

char getc(){
    uint64 brSisPoziva=0x41;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));
    __asm__ volatile("ecall");
    char c;
    __asm__ volatile ("mv %[ptr], a0":[ptr]"=r"(c));

    return c;
}
void putc(char c){
    __asm__ volatile("mv a1,%[pointer]"::[pointer] "r"(c));
    uint64 brSisPoziva=0x42;
    __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(brSisPoziva));

    __asm__ volatile("ecall");
   
}
void user_mode(){

	uint64 num = 0x43;
	__asm__ volatile("mv a0, %[num]" : : [num] "r"(num));
	__asm__ volatile("ecall");
}
void system_mode(){
	uint64 num = 0x44;
	
	__asm__ volatile("mv a0, %[num]" : : [num] "r"(num));
	__asm__ volatile("ecall");
}

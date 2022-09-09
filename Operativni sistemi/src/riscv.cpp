//
// Created by os on 8/9/22.
//



#include "../h/riscv.hpp"
#include "../h/CCB.hpp"
#include "../lib/console.h"
#include "../h/printing.hpp"

void Riscv::popSppSpie(){

    __asm__ volatile("csrw sepc, ra");
    ms_sstatus(SSTATUS_SPP);
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{

    uint64 sstatus=r_sstatus();
    uint64 povecajza4;
    povecajza4=r_sepc()+4;
    //w_sepc(povecajza4);
    uint64 razlog;
    __asm__ volatile("mv %[razlog], a0":[razlog] "=r"(razlog));
    uint64 scause=r_scause();
    if(scause==0x09 || scause == 0x08){

            if(razlog==0x11){//thread_create koji stavlja u scheduler
                thread_t *handle;
                void (*start_routine)(void *);
                void *arg;

                __asm__ volatile("mv %[arg1], a1":[arg1] "=r"(handle));
                __asm__ volatile("mv %[arg2], a2":[arg2] "=r"(start_routine));
                __asm__ volatile("mv %[arg3], a6":[arg3] "=r"(arg));
                CCB * novi=new CCB(handle,start_routine,arg);
                if(novi== nullptr){
                    int c=-5;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }else{
                    int c=0;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                Scheduler::put(novi);
                }
            }else if(razlog==0x14){//thread_start
                thread_t handle;
                __asm__ volatile("mv %[arg1], a1":[arg1] "=r"(handle));
                handle->t_start();
            }else if(razlog==0x12){//thread_exit
                if(CCB::running== nullptr){
                    int c=-5;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }else {
                    CCB::running->setFinished(true);
                    CCB::yield();
                    int c=0;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }

            } else if(razlog==0x13){//thread_dispatch
                CCB::yield();
            } else if(razlog==0x01){//mem_alloc
                uint64 size;
                __asm__ volatile("mv %[arg1], a1":[arg1] "=r"(size));
                MemoryAllocator::mem_alloc(size);
                __asm__ volatile("sd a0, 80(s0)");//__asm__ volatile("mv a1,%[sisPoziv]"::[sisPoziv] "r"(ptr));
            } else if(razlog==0x02){//mem_free
                void*p;
                __asm__ volatile("mv %[arg1], a1":[arg1] "=r"(p));
                MemoryAllocator::mem_free(p);
            }else if(razlog==0x21){//sem_open
                sem_t* handle;
                uint64 init;
                __asm__ volatile("mv %[arg1], a1":[arg1] "=r"(handle));
                __asm__ volatile("mv %[arg2], a2":[arg2] "=r"(init));
                if(new _sem(handle,init)== nullptr){
                    int c=-5;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }else {
                    int c=0;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }
            }else if(razlog==0x22){//sem_close
                sem_t id;
                __asm__ volatile("mv %[arg1], a1":[arg1] "=r"(id));
                if(id== nullptr){
                    int c=-5;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }else{
                    id->s_close();
                    int c=0;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }

            }else if(razlog==0x23){//sem_wait
                sem_t id;
                __asm__ volatile("mv %[arg1], a1":[arg1] "=r"(id));

                if(id== nullptr|| CCB::running->isDeleted()){
                    int c=-5;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }else{
                    id->s_wait();
                    int c=0;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }
            }else if(razlog==0x24){//sem_signal
                sem_t id;
                __asm__ volatile("mv %[arg1], a1":[arg1] "=r"(id));

                if(id== nullptr){
                    int c=-5;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }else{
                    id->s_signal();
                    int c=0;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }
            }else if(razlog==0x15){//thread_create1 koji ne stavlja u Scheduler
                thread_t *handle;
                void (*start_routine)(void *);
                void *arg;

                __asm__ volatile("mv %[arg1], a1":[arg1] "=r"(handle));
                __asm__ volatile("mv %[arg2], a2":[arg2] "=r"(start_routine));
                __asm__ volatile("mv %[arg3], a6":[arg3] "=r"(arg));
                if(new CCB(handle,start_routine,arg)== nullptr){
                    int c=-5;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }else {
                    int c=0;
                    __asm__ volatile("mv a5,%[sisPoziv]"::[sisPoziv] "r"(c));
                }
            }else if(razlog==0x41){

                char c= __getc();
               
                __asm__ volatile("mv a0,%[sisPoziv]"::[sisPoziv] "r"(c));
                __asm__ volatile("sd a0, 80(s0)");
            }else if(razlog==0x42){//putc
                
                char c;
                __asm__ volatile("mv %[arg1], a1":[arg1] "=r"(c));
                __putc(c);
                
            }
            else if(razlog==0x43){
            	mc_sstatus(SSTATUS_SPP);
            	w_sepc(povecajza4);
            	return;
            }
            else if(razlog == 0x44){
            	ms_sstatus(SSTATUS_SPP);
            	w_sepc(povecajza4);
            	return;
            }
            
            w_sepc(povecajza4);
    		w_sstatus(sstatus);

        }
    else if(scause == 0x8000000000000001UL){

            Riscv::mc_sip(SIP_SSIP);
        }
    else if(scause == 0x8000000000000009UL)
        {
        console_handler();
    }else{
        printInt(scause);
        printString("\n");
    }
    
}

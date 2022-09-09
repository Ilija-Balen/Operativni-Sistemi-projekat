//
// Created by os on 8/5/22.
//
/*#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h
 #include "../h/alloc.hpp"
#include "../h/MemoryAllocator.hpp"*/

/*class MemoryAllocator{
private:
    char a;
public:
    MemoryAllocator(){
        a='a';
    }
    char f(){
        return a;
    }
};*/
//#include "userMain.cpp"
#include "../h/CCB.hpp"
#include "../h/printing.hpp"
extern "C" void supervisorTrap();
void userMain();


void userMainWrapper(void*)
{
	userMain();
}

void main(){

    asm volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(supervisorTrap));
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    CCB *mainkorutina;
    CCB* usermainkorutina;
    mainkorutina = CCB::createCoroutine(nullptr);
    CCB::running=mainkorutina;

    usermainkorutina = CCB::createCoroutine(userMainWrapper);
    Scheduler::put(usermainkorutina);
    
    user_mode();

    while(!usermainkorutina->isFinished()) thread_dispatch();
    
    system_mode();

}

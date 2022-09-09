//
// Created by os on 8/15/22.
//

#ifndef PROJECT_BASE__SEM_HPP
#define PROJECT_BASE__SEM_HPP
#include "CCB.hpp"


class _sem {
private:
    int init;
    mojaLista<CCB> blokiraneNiti;
public:
    _sem(sem_t* handle, uint64 i){
        *handle=this;
        init=(int)i;

    }
    void* operator new (size_t size);
    void operator delete(void* ptr);

    void s_signal();
    void s_wait();
    void s_close();
};


#endif //PROJECT_BASE__SEM_HPP

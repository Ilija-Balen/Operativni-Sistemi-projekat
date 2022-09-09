//
// Created by os on 8/10/22.
//

#ifndef PROJECT_BASE_MOJALISTA_H
#define PROJECT_BASE_MOJALISTA_H
#include "MemoryAllocator.hpp"
template <typename T>
class mojaLista {
private:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };
    Elem *head, *tail;
public:
    mojaLista(){
        head=tail= nullptr;

    }
    void dodaj(T* data);
    T* izvadi();
};

template<typename T>
void mojaLista<T>::dodaj(T *data) {
    Elem *elem =(Elem*)MemoryAllocator::mem_alloc(sizeof (Elem)) ;//new Elem(data, head)
    elem->data=data;
    elem->next=head;
    head = elem;
    if (!tail) { tail = head; }
}

template<typename T>
T *mojaLista<T>::izvadi() {
    if (!head) { return 0; }

    Elem *prev = 0;
    for (Elem *curr = head; curr && curr != tail; curr = curr->next)
    {
        prev = curr;
    }

    Elem *elem = tail;
    if (prev) { prev->next = 0; }
    else { head = 0; }
    tail = prev;

    T *ret = elem->data;
    MemoryAllocator::mem_free(elem);//delete elem;
    return ret;
}


#endif //PROJECT_BASE_MOJALISTA_H

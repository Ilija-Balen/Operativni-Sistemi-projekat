//
// Created by os on 8/8/22.
//

#include "../h/MemoryAllocator.hpp"
MemoryAllocator::MemorySegmentHeader* MemoryAllocator::prviSlobodan=0;
void MemoryAllocator::inicijalizuj(uint64 pocetna, uint64 velicina) {
    prviSlobodan = (MemorySegmentHeader *) pocetna;
    prviSlobodan->length = velicina - sizeof(MemorySegmentHeader);
    prviSlobodan->free = true;

    prviSlobodan->NextSegment = 0;
    prviSlobodan->PreviousSegment = 0;
}

void *MemoryAllocator::mem_alloc(size_t size) {
    if (prviSlobodan == 0)
        inicijalizuj((uint64) HEAP_START_ADDR, (uint64) HEAP_END_ADDR - (uint64) HEAP_START_ADDR - 1);
    uint64 ostatak = size % MEM_BLOCK_SIZE; //kako bi sve bilo poravnato na MEM_BLOCK_SIZE bajtova
    size -= ostatak;
    if (ostatak != 0)size += MEM_BLOCK_SIZE;

    MemorySegmentHeader *current = prviSlobodan;
    while (true) {
        if (current->length >= size && current->free == true) {
            if (current->length > size + sizeof(MemorySegmentHeader)) { //podeli memorijski segment
                uint64 a = sizeof(MemorySegmentHeader) + size;
                MemorySegmentHeader *novi = (MemorySegmentHeader *) ((uint64) current + a);


                novi->free = true;
                novi->length = ((uint64) current->length) - (sizeof(MemorySegmentHeader) + size);
                novi->NextSegment = 0;
                novi->PreviousSegment = current;

                current->NextSegment = novi;
                current->length = size;

            }
            if (current == prviSlobodan) {
                prviSlobodan = current->NextSegment;
            }
            current->free = false;

            return current + 1;//posto koristimo MemorySegmentHeader pokazivac svako inkrementriranje istog ce nam pomeriti pokazivac za sizeof(memorysegmentheadera)
        }
        if (current->NextSegment == 0) {//ukoliko nema vise memorije ovo se poziva
            return 0;
        }
        current = current->NextSegment;
    }
    return nullptr;
}
void MemoryAllocator::combineFree(MemorySegmentHeader *a, MemorySegmentHeader *b) {
    if (b == 0)return;
    if (a == 0)return;

    if (a < b) {
        a->length += b->length + sizeof(MemorySegmentHeader);

        a->NextSegment = b->NextSegment;
        if (b->NextSegment != 0)//zastita za poslednji deo heapa
            b->NextSegment->PreviousSegment = a;

    } else {
        b->length += a->length + sizeof(MemorySegmentHeader);

        b->NextSegment = a->NextSegment;
        if (a->NextSegment != 0)
            a->NextSegment->PreviousSegment = b;

    }
}
int MemoryAllocator::mem_free(void *ptr) {
    //lock();
    if(ptr== nullptr) return 0;
    MemorySegmentHeader *curr = ((MemorySegmentHeader *) ptr) -1;//isto vazi sto je i gore vazilo, smanjimo za sizeof memorysegmentheader
    curr->free = true;

    if (curr < prviSlobodan)prviSlobodan = curr;

    if (curr->NextSegment != 0)
        if (curr->NextSegment->free)
            combineFree(curr, curr->NextSegment);

    if (curr->PreviousSegment != 0)
        if (curr->PreviousSegment->free)
            combineFree(curr, curr->PreviousSegment);

    curr = 0;
    return 0;
    //unlock();
}

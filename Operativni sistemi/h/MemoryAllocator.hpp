//
// Created by os on 8/8/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP
#include "../lib/hw.h"

class MemoryAllocator {
private:

    MemoryAllocator(){}
     struct MemorySegmentHeader{
        uint64 length;
        MemorySegmentHeader *NextSegment;
        MemorySegmentHeader *PreviousSegment;
        bool free;
    };
    static MemorySegmentHeader*prviSlobodan;
public:
    static void combineFree(MemorySegmentHeader *a, MemorySegmentHeader *b);
    static void inicijalizuj(uint64 pocetna, uint64 velicina);
    static void *mem_alloc(size_t size);
    static int mem_free(void *ptr);
};


#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP

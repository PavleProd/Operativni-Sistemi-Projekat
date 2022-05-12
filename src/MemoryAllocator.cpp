#include "../h/MemoryAllocator.h"

MemoryAllocator::FreeSegment* MemoryAllocator::head = nullptr;

void *MemoryAllocator::mem_alloc(size_t size) {
    if(head == nullptr) { // ako prvi put alociramo memoriju, alociracemo head na pocetak mem segmenta (koji je slobodan)
        head = (FreeSegment*)HEAP_START_ADDR;
        head->baseAddr = (void*)HEAP_START_ADDR;
        head->size = ((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR + 1);
        head->next = nullptr;
    }
    else if(head == (FreeSegment*)HEAP_END_ADDR) { // ako ne postoji slobodan prostor
        return nullptr;
    }

    size_t numOfBlocksToAllocate = sizeInBlocks(size);

    FreeSegment *curr = head, *prev = nullptr;
    while(curr) {
        size_t freeSegSizeInBlocks = sizeInBlocks(curr->size);
        size_t allocatedSize = 0;
        void* startOfAllocatedSpace = curr->baseAddr;
        if(freeSegSizeInBlocks >= numOfBlocksToAllocate) {
            if(freeSegSizeInBlocks == numOfBlocksToAllocate) { // ako ce preostali slobodan segment da bude manji od jednog bloka onda i njega dodeljujemo memoriji
                allocatedSize = curr->size;

                if(prev == nullptr) { // ako smo na head pokazivacu
                    if(curr->next == nullptr) { // ako ne postoji slobodan prostor nakon alokacije
                        head = (FreeSegment*)HEAP_END_ADDR;
                    }
                    else {
                        head = curr->next;
                    }
                }
                else {
                    FreeSegment::remove(prev);
                }
            }
            else {
                allocatedSize = blocksInSize(numOfBlocksToAllocate);
                void* newBaseAddr = ((char*)startOfAllocatedSpace + allocatedSize);
                size_t newSize = curr->size - allocatedSize;

                FreeSegment* newSeg = (FreeSegment*)newBaseAddr;
                newSeg->baseAddr = newBaseAddr;
                newSeg->size = newSize;
                newSeg->next = curr->next;

                if(!prev) {
                    head = newSeg;
                }
                else {
                    FreeSegment::remove(prev);
                    FreeSegment::add(prev, curr);
                }
            }

            return startOfAllocatedSpace;
        }

        prev = curr;
        curr = curr->next;
    }

    return nullptr; // ne postoji dovoljan slobodan prostor
}

int MemoryAllocator::mem_free(void *memSegment) {
    return 0;
}
#include "../h/PCB.h"
#include "../h/Scheduler.h"
#include "../h/MemoryAllocator.h"
#include "../h/kernel.h"
#include "../h/syscall_c.h"

PCB* PCB::running = nullptr;
size_t PCB::timeSliceCounter = 0;

PCB *PCB::createProccess(PCB::processMain main, void* arguments) {
    return new PCB(main, DEFAULT_TIME_SLICE, arguments);
}

void PCB::yield() {
    size_t code = Kernel::sysCallCodes::thread_dispatch;
    asm volatile("mv a0, %0" : : "r" (code));
    asm volatile("ecall"); // prelazimo u prekidnu rutinu koja cuva registre i vrsi promenu konteksta
}

void PCB::dispatch() {
    PCB* old = running;
    if(!old->isFinished() && !old->blocked) { // ako se nije zavrsio i nije blokiran
        Scheduler::put(old);
    }
    running = Scheduler::get();

    if(PCB::running->firstCall) {
        PCB::running->firstCall = false;
        switchContext1(old->registers, running->registers);
    }
    else {
        switchContext2(old->registers, running->registers);
    }

}

// main_ == nullptr ako smo u glavnom procesu
PCB::PCB(PCB::processMain main_, size_t timeSlice_, void* mainArguments_) {
    finished = blocked = false;
    main = main_;
    timeSlice = timeSlice_;
    mainArguments = mainArguments_;
    registers = (size_t*)MemoryAllocator::mem_alloc(33*sizeof(size_t)); // mozemo direktno jer se zove iz sistemskog rezima samo
    sysStack = (size_t*)MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE*sizeof(size_t));
    registers[0] = (size_t)&sysStack[DEFAULT_STACK_SIZE]; // ssp postavljamo na vrh steka
    registers[32] = (size_t)&proccessWrapper; // u ra cuvamo proccessWrapper
    stack = nullptr; // stek pravimo u sistemskom pozivu thread_create
    if(main == nullptr) firstCall = false; //  jer je glavni proces vec pokrenut, necemo ici u process_wrapper
}

PCB::~PCB() {
    delete[] stack;
    delete[] sysStack;
}

void *PCB::operator new(size_t size) {
    return MemoryAllocator::mem_alloc(size);
}

void PCB::operator delete(void *memSegment) {
    MemoryAllocator::mem_free(memSegment);
}

void PCB::proccessWrapper() { // iz prekidne rutine skacemo ovde kada prvi put ulazimo u nit
    Kernel::popSppSpie(); // izlazimo iz prekidne rutine
    void* arg = PCB::running->mainArguments;
    asm volatile("mv a0, %0" : : "r" (arg)); // u a0 postavljamo argument glavne funkcije procesa
    running->main();
    thread_exit();
}

size_t *PCB::getContext() {
    return running->registers;
}

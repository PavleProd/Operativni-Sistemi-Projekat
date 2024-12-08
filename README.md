# Operating Systems Projects / Projekat iz Operativnih sistema

This project implements a small but fully functional kernel for a lightweight operating system targeting the RISC-V architecture. The kernel supports multithreading with **time-sharing**, basic memory allocation, semaphores, and asynchronous context switching triggered by a timer or keyboard interrupts.

It is a project on the course Operating Systems 1 ("Operativni Sistemi 1") on University of Belgrade, Faculty of Electrical Engineering, Module for Computer Engineering and Informatics.
It was later upgraded on the course Operating Systems 2 ("Operativni Sistemi 2") where the project was to implement Buddy Allocator

---

## Part 1: Operating Systems 1 - Kernel Implementation

### Overview
The goal of the first project is to implement a basic operating system kernel with the following functionalities:
- **Thread Management**:
  - Support for lightweight threads with preemptive time-sharing.
- **Memory Management**:
  - Basic block-based memory allocation and deallocation.
- **Synchronization**:
  - Semaphores for thread coordination.
- **Interrupt Handling**:
  - Handling timer and keyboard interrupts.
- **System Calls**:
  - Providing a layered interface (C API, ABI, and C++ API) for interaction between user programs and the kernel.

### Key Features
1. **Thread Management**:
   - **Thread Creation**: Create threads for user-defined functions (`thread_create`).
   - **Context Switching**:
     - Synchronous (`thread_dispatch`) and asynchronous (interrupt-driven).
   - **Thread Termination**: Cleanly terminate threads (`thread_exit`).

2. **Memory Management**:
   - **Dynamic Memory Allocation**: Allocate memory blocks (`mem_alloc`) aligned to block sizes.
   - **Memory Deallocation**: Free previously allocated memory (`mem_free`).

3. **Synchronization Mechanisms**:
   - Semaphore-based synchronization (`sem_open`, `sem_wait`, `sem_signal`, `sem_close`).

4. **Interrupt Handling**:
   - Handle timer interrupts for preemption and keyboard interrupts for I/O.

5. **Layered Interfaces**:
   - **C API**: Procedural interface for kernel services.
   - **ABI**: Low-level interface using software interrupts for efficient kernel communication.
   - **C++ API**: Object-oriented abstraction for threads and semaphores.

6. **Console I/O**:
   - Simple character input/output (`getc`, `putc`) through system calls.

---

## Part 2: Operating Systems 2 - Buddy Allocator Implementation

### Overview
The second project builds upon the kernel developed in the first project, extending its memory management system by introducing a **Buddy Allocator**. The Buddy Allocator improves memory allocation efficiency and minimizes fragmentation by implementing a binary tree-based allocation system.

### Key Features
1. **Buddy Allocation Algorithm**:
   - Divide memory into blocks of sizes that are powers of two.
   - Merge adjacent free blocks (buddies) when possible to form larger blocks.
   - Split larger blocks to satisfy smaller allocation requests.

2. **Memory Management Functions**:
   - `buddy_alloc(size_t size)`: Allocates memory of the requested size, rounded to the nearest power of two.
   - `buddy_free(void* ptr)`: Deallocates previously allocated memory and attempts to merge free blocks.

3. **Optimized Memory Usage**:
   - Reduces internal and external fragmentation.
   - Efficiently supports both large and small allocation requests.

4. **Integration with Kernel**:
   - The Buddy Allocator replaces the basic block-based allocator from the first project.
   - All kernel and user-space memory allocations are managed through this allocator.

# Thread-Safe Queues (in C++)

## Goal [WIP]
Aim is to create a mini project to build thread safe queues in C++. Idea is:
1. Create thread safe in-memory queues using several concurrent constructs. Goal is to code in C++, but use other language to explain concurrent constructs not available in c++ (like software transactional memory in haskell)
2. Create a thread safe disk-based queues with reasonable guarentees on durability (i.e durable under certain (preferably) controlled conditions)
3. create a network based synchrnous clients to push and pop into the queue. (At this point, we will have our own mini rabbitmq)
4. create a mini kafka like system with reasonable guarentees on delivery semantics and durabilty
5. Create or use existing performance tools to investigate above things.

## Theory

In a concurrent system, a queue can be a shared resource, necessitating thread-safe access. Concurrent systems can involve:

1.	*User-space threads*: These are mapped 1:1 to kernel threads. Examples include pthreads, std::threads,
      and the Java Thread class.
2.	*Green threads or lightweight threads*: These are managed by the programming language runtime, with an M:N mapping
      between green threads and user-space threads, which in turn map to kernel-level threads through syscalls. Examples include Go coroutines and Haskell lightweight threads.

To avoid data race conditions or liveness/deadlock problems in concurrent systems, several solutions are available:

1.	Using Mutexes (Mutual Exclusion): Locks and unlock functions, supported by hardware, ensure atomic operations.
	•	Note: Threads might be blocked if the lock is unavailable.
2.	Using Lock-free Data Structures: Data structures are defined using atomic operations and compare-and-swap constructs provided
      by the hardware.
3.	Using Software Transactional Memory (STM): A feature implemented in some programming languages, like Haskell.

This document discusses and provides code and tests for the first two methods (mutexes and lock-free data structures) in C++. It will also briefly cover software transactional memory in Haskell and its applicability in C++.

## General Queue APIs

An abstract queue typically supports the following operations:

1.	push(item): Pushes an item into the queue.
2.	pop(): Returns an item from the queue.

Additionally, the following operations are often necessary:

3.	is_empty(): Checks if the queue is empty.
4.	size(): Returns the current size of the queue.
5.	front(): Peeks at the first element of the queue without popping it.
6.	is_full(): Checks if the queue is full.

### APIs for Thread-Safe Queues

Designing APIs for thread-safe queues involves several considerations:

1.	*Validity of General Queue APIs:* Are the general queue APIs valid for a concurrent queue as well?
      - APIs is_empty(), size(), front(), is_full() could be made thread safe without race conditions but a valid value returned by these functions might be invalid, if any other thread changes the structure of the queue (i.e push or pop)
2.	*Impact of Underlying Data Structure:* Do the APIs change based on the underlying data structure of the queue?
3.	*T*hread Behavior:* Should threads be blocked until the resource is available, or should there be additional APIs
      to allow threads to decide what to do if the resource is unavailable?


# Current Status
The code has two threadsafe queues

1. Implement a thread safe wrapper over std::queue
    - we don't worry about underlying datastructure
    - the memory limits are system memory limits, so we call this *unbounded queue*
    - we will check for this case for meaningful error messages while enqueue and if system is out of memory
2. Implement a thread safe queue of fixed size,  that is to be allocated at front.
     - This means the underlying datastructure is a vector of fixed capacity. we call this *bounded queue*
     - Ofcourse, we could have all complicated implementatons of allocating fixed size heap and use linked list from ths memory, like that of a working set memory (sort of mini memory pool). But we won't get into that.

These thread are used by concurrent threads to enqueue and dequeue (which pops the queue as well).

## CONSIDERATIONS:
1) use c++11 or c++14 constructs. No pthreads are allowed. Use safe C++ (for example, using lock_guards, unique_locks or c++ safe pointer mechanisms or atomic counters if possible)
2) Avoid notifying all threads to wake up as it's unnecessarly wastes CPU cycles. 

## SUPPORT
1. supports MacOS (Arm64) with Clang and Linux with gcc.(Tested on MacOS only. But should work for linux)


NOTE: We could improve the performance of the overall system, if we there are some threads reading only the front of the queue, using RW locks (or shared mutex for C++17)

## How to run
- `./test_threadsafequeues -h` for help message
## help
```shell
Usage: ./test_threadsafequeues [OPTIONS]
Options:
  -P, --producers <N>       Number of producers (default: 1)
  -C, --consumers <N>       Number of consumers (default: 1)
  -N, --num-items <N>       Number of items to produce/consume (default: 10)
  -S, --sleep-time <N>      Sleep time in milliseconds (default: 10)
  -B, --queue-capacity <N>  Queue capacity for bounded queue (default: 100)
  -T, --queue-type <type>   Queue type (unbounded or bounded, default: unbounded)
  -F, --thread-safe-cout    Thread-safe console output. If enabled, cout is thread safe (default:false)
  -h, --help                Show this help message
```

## Sample Output

Below is the output when we make std:cout as thread safe. Even then it is NOT guanrenteed that the output we see shows the consumers print the queue items in the order they consumed 
```shell
     ./test_threadsafequeues -P5 -C5 -N1 --thread-safe-cout
Testing unbounded queue...
Produced by producer thread: 0 item: {0, 0} (c++ thread id): 0x16f21b000
Produced by producer thread: 3 item: {3, 3} (c++ thread id): 0x16f3bf000
Produced by producer thread: 4 item: {4, 4} (c++ thread id): 0x16f44b000
Produced by producer thread: 2 item: {2, 2} (c++ thread id): 0x16f333000
######  Consumeed by consumer thread: 0 item: {producer_id: 0, 0} (c++ thread id): 0x16f4d7000
Produced by producer thread: 1 item: {1, 1} (c++ thread id): 0x16f2a7000
######  Consumeed by consumer thread: 2 item: {producer_id: 4, 4} (c++ thread id): 0x16f5ef000
######  Consumeed by consumer thread: 3 item: {producer_id: 3, 3} (c++ thread id): 0x16f67b000
######  Consumeed by consumer thread: 4 item: {producer_id: 2, 2} (c++ thread id): 0x16f707000
######  Consumeed by consumer thread: 1 item: {producer_id: 1, 1} (c++ thread id): 0x16f563000
Unbounded queue test completed.
```






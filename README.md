# ThreadSafeQueues
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
1) 
use c++11 or c++14 constructs. No pthreads are allowed. Use safe C++ (for example, using lock_guards, unique_locks or c++ safe pointer mechanisms or atomic counters if possible)
2) Avoid notifying all threads to wake up as it's unnecessarly wastes CPU cycles. 


NOTE: We could improve the performance of the overall system, if we there are some threads reading only the front of the queue, using RW locks (or shared mutex for C++17)

# How to run
- `./test_threadsafequeue -h` for help message



# Future improvements

Obviously lot to improve, this queue is memory based. One could make a small changes, that makes queue persist to disk
<TODO> write a simple mechanism to implement mini rabbitMQ that's flushes to the disk. 




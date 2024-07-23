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
1) use c++11 or c++14 constructs. No pthreads are allowed. Use safe C++ (for example, using lock_guards, unique_locks or c++ safe pointer mechanisms or atomic counters if possible)
2) Avoid notifying all threads to wake up as it's unnecessarly wastes CPU cycles. 

## SUPPORT
1. supports MacOS (Arm64) with Clang and Linux with gcc.(Tested on MacOS only. But should work for linux)


NOTE: We could improve the performance of the overall system, if we there are some threads reading only the front of the queue, using RW locks (or shared mutex for C++17)

# How to run
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



# Future improvements
1. APIs for thread-safe and non-thread safe differ now i.e eneque/dequeue for thread-safe and push/pop for non-thread-safe. Similarly to python
2. Test memory limits for unbounded queue using simulation testiing
3. serialize queue output to a file in the graceful shutdown and deserialize while looking out
   This will make a mini rabbitmq because of this conditoned persistence





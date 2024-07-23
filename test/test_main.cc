#include "../lib/queues/unbounded_queue.h"   //TODO: ugly: make it a separate librayr to link later
#include "../lib/queues/bounded_queue.h"
#include "options.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

/**
 * @brief Struct to represent an item in the queue.
 * 
 * @tparam T type-agnostic data
 */
template<typename T>
struct queue_item {
    int thread_id; ///< The ID of the thread that produced the item.
    T data; ///< The data produced by the thread.
};

/**
 * mutex for std::cout to display better output when several threads print
 */
std::mutex cout_mutex;

/**
 * 
 */


/**
 * @brief Producer function for the queue.
 * 
 * @tparam queue_type The type of the queue.
 * @param queue The queue to produce items to.
 * @param id The producer ID.
 * @param items_to_produce The number of items to produce.
 * @param sleep_time_ms The time to sleep between producing items.
 * @param thread_safe_cout flag to lock std::cout
 */
template <typename queue_type>
void producer(queue_type& queue, int id, int items_to_produce, int sleep_time_ms, bool thread_safe_cout) {
    for (int i = 0; i < items_to_produce; ++i) {
        queue_item<int> item = { id, i + id * items_to_produce };

        if (thread_safe_cout){
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Produced by producer thread: " << id <<
            " item: {" << item.thread_id << ", " << item.data << "}" << " (c++ thread id): " << std::this_thread::get_id() << std::endl;
        }else {
            std::cout << "Produced by producer thread: " << id <<
            " item: {" << item.thread_id << ", " << item.data << "}" << " (c++ thread id): " << std::this_thread::get_id() << std::endl;
        }

        queue.enqueue(item);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms)); // simulate
    }
}

/**
 * @brief Consumer function for the queue.
 * 
 * @tparam queue_type The type of the queue.
 * @param queue The queue to consume items from.
 * @param items_to_consume The number of items to consume.
 * @param sleep_time_ms The time to sleep between consuming items.
 * @param thread_safe_cout flag to lock std::cout
 */
template <typename queue_type>
void consumer(queue_type& queue, int id, int items_to_consume, int sleep_time_ms, bool thread_safe_cout) {
    for (int i = 0; i < items_to_consume; ++i) {
        queue_item<int> item;
        queue.dequeue(item);

        if (thread_safe_cout){
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "######  Consumeed by consumer thread: " << id <<
            " item: {" << "producer_id: " << item.thread_id << ", " << item.data << "}" << " (c++ thread id): " << std::this_thread::get_id() << std::endl;
        }else {
            std::cout << "####### Consummed by consumer thread: " << id <<
            " item: {" << "producer_id: " << item.thread_id << ", " << item.data << "}" << " (c++ thread id): " << std::this_thread::get_id() << std::endl;

        }
       
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms)); // simulate work
    }
}

/**
 * @brief Tests the unbounded queue.
 * 
 * @param options The program options.
 */
void test_unbounded_queue(const program_options& options) {
    unbounded_queue<queue_item<int>> ub_queue;

    std::vector<std::thread> producer_threads;
    std::vector<std::thread> consumer_threads;

    std::cout << "Testing unbounded queue..." << std::endl;
    for (int i = 0; i < options.num_producers; ++i) {
        producer_threads.emplace_back(producer<unbounded_queue<queue_item<int>>>, std::ref(ub_queue), i, options.num_items, options.sleep_time_ms, options.thread_safe_cout);
    }

    for (int i = 0; i < options.num_consumers; ++i) {
        consumer_threads.emplace_back(consumer<unbounded_queue<queue_item<int>>>, std::ref(ub_queue), i, options.num_items, options.sleep_time_ms, options.thread_safe_cout);
    }

    for (auto& t : producer_threads) {
        t.join();
    }

    for (auto& t : consumer_threads) {
        t.join();
    }

    std::cout << "Unbounded queue test completed." << std::endl;
}

/**
 * @brief Tests the bounded queue.
 * 
 * @param options The program options.
 */
void test_bounded_queue(const program_options& options) {
    bounded_queue<queue_item<int>> b_queue(options.queue_capacity);

    std::vector<std::thread> producer_threads;
    std::vector<std::thread> consumer_threads;

    std::cout << "Testing bounded queue..." << std::endl;
    for (int i = 0; i < options.num_producers; ++i) {
        producer_threads.emplace_back(producer<bounded_queue<queue_item<int>>>, std::ref(b_queue), i, options.num_items, options.sleep_time_ms, options.thread_safe_cout);
    }

    for (int i = 0; i < options.num_consumers; ++i) {
        consumer_threads.emplace_back(consumer<bounded_queue<queue_item<int>>>, std::ref(b_queue), i, options.num_items, options.sleep_time_ms, options.thread_safe_cout);
    }

    for (auto& t : producer_threads) {
        t.join();
    }

    for (auto& t : consumer_threads) {
        t.join();
    }

    std::cout << "Bounded queue test completed." << std::endl;
}

/**
 * @brief Main function to run the queue tests.
 * 
 * Parses command line arguments and runs the appropriate queue test based on the options provided.
 * 
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return int Exit status of the program.
 */
int main(int argc, char* argv[]) {
    program_options options;

    parse_command_line_arguments(argc, argv, options);

    if (options.queue_type == "unbounded") {
        test_unbounded_queue(options);
    } else if (options.queue_type == "bounded") {
        test_bounded_queue(options);
    } else {
        std::cerr << "Invalid queue type. Use 'unbounded' or 'bounded'." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
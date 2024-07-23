#ifndef UNBOUNDED_QUEUE_H
#define UNBOUNDED_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

/**
 * @brief A thread-safe unbounded queue.
 * 
 * This class provides a thread-safe unbounded queue implementation using std::queue.
 * It uses a mutex and condition variable to ensure thread safety.
 * 
 * @tparam T The type of elements stored in the queue.
 */
template<typename T>
class unbounded_queue {
public:
  //  using value_type = T; ///< Type of elements stored in the queue
    /**
     * @brief Enqueues an item into the queue.
     * 
     * @param item The item to be enqueued.
     */
    void enqueue(const T& item);

    /**
     * @brief Dequeues an item from the queue.
     * 
     * @param item The item to be dequeued.
     */
    void dequeue(T& item);

    /**
     * @brief Checks if the queue is empty.
     * 
     * @return true if the queue is empty, false otherwise.
     */
    bool is_empty() const;

    /**
     * @brief Returns the size of the queue.
     * 
     * @return The number of elements in the queue.
     */
    size_t size() const;

private:
    std::queue<T> m_queue; ///< underlying queue
    mutable std::mutex m_mutex; 
    std::condition_variable m_cond; 
};

template<typename T>
void unbounded_queue<T>::enqueue(const T& item) {
    std::unique_lock<std::mutex> lock(m_mutex);
    try {
        m_queue.push(item);
        m_cond.notify_one();
    } catch (const std::bad_alloc&) {
        std::cerr << "enqueue operation cannot succeed because system memory has been reached" << std::endl;
    }
}

template<typename T>
void unbounded_queue<T>::dequeue(T& item) {
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_queue.empty()) {
        m_cond.wait(lock);
    }
    item = m_queue.front();
    m_queue.pop();
}

template<typename T>
bool unbounded_queue<T>::is_empty() const {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_queue.empty();
}

template<typename T>
size_t unbounded_queue<T>::size() const {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_queue.size();
}

#endif // UNBOUNDED_QUEUE_H
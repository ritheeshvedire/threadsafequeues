#ifndef BOUNDED_QUEUE_H
#define BOUNDED_QUEUE_H

#include <vector>
#include <mutex>
#include <condition_variable>

/**
 * @brief A thread-safe bounded queue over std::queue which is initalized wth capacity
 * 
 * Underlying std::queue is a vector. Takes capacity as input
 * It uses a mutex and condition variables to ensure thread safety.
 * 
 * @tparam T The type of elements stored in the queue.
 */
template<typename T>
class bounded_queue {
public:
    using value_type = T; ///< Type of elements stored in the queue
    /**
     * @brief Constructs a bounded queue with the given capacity.
     * 
     * @param capacity The maximum number of elements the queue can hold.
     */
    explicit bounded_queue(size_t capacity);

    /**
     * @brief Enqueues an item into the queue.
     * 
     * @param item The item to be enqueued.
     * @return true if the item was enqueued successfully, false if the queue was full.
     */
    bool enqueue(const T& item);

    /**
     * @brief Dequeues an item from the queue.
     * 
     * @param item The item to be dequeued.
     * @return true if the item was dequeued successfully, false if the queue was empty.
     */
    bool dequeue(T& item);

    /**
     * @brief Checks if the queue is full.
     * 
     * @return true if the queue is full, false otherwise.
     */
    bool is_full() const;

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
    std::vector<T> m_queue; ///< underlying queue.
    size_t m_capacity; 
    size_t m_front;
    size_t m_back; 
    size_t m_count; 
    mutable std::mutex m_mutex; 
    std::condition_variable m_not_full; ///< when the queue is full, threads wait to receive notificaton of queue not being full
    std::condition_variable m_not_empty; ///< when the queue is empty, threads wait to receive notificaton of queue not being empty 
};

template<typename T>
bounded_queue<T>::bounded_queue(size_t capacity)
    : m_capacity(capacity), m_front(0), m_back(0), m_count(0) {
    m_queue.resize(capacity);
}

template<typename T>
bool bounded_queue<T>::enqueue(const T& item) {
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_count == m_capacity) {
        m_not_full.wait(lock);
    }
    m_queue[m_back] = item;
    m_back = (m_back + 1) % m_capacity;
    ++m_count;
    m_not_empty.notify_one();
    return true;
}

template<typename T>
bool bounded_queue<T>::dequeue(T& item) {
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_count == 0) {
        m_not_empty.wait(lock);
    }
    item = m_queue[m_front];
    m_front = (m_front + 1) % m_capacity;
    --m_count;
    m_not_full.notify_one();
    return true;
}

template<typename T>
bool bounded_queue<T>::is_full() const {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_count == m_capacity;
}

template<typename T>
bool bounded_queue<T>::is_empty() const {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_count == 0;
}

template<typename T>
size_t bounded_queue<T>::size() const {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_count;
}

#endif // BOUNDED_QUEUE_H
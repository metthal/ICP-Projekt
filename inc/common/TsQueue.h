#ifndef TSQUEUE_H
#define TSQUEUE_H

#include "Shared.h"
#include <queue>
#include <boost/thread.hpp>

template <typename T> class TsQueue
{
    typedef T                       value_type;
    typedef value_type&             reference;
    typedef const value_type&       const_reference;
    typedef size_t                  size_type;

public:
    explicit TsQueue(const std::queue<value_type>& = std::queue<value_type>()) : m_interrupted(false) {}

    TsQueue(const TsQueue &queue)
    {
        queue.m_interrupted = m_interrupted;
        queue.m_queue = m_queue;
    }

    ~TsQueue() {}

    bool empty()
    {
        ScopedLock lock(m_mutex);

        return m_queue.empty();
    }

    size_type size()
    {
        ScopedLock lock(m_mutex);

        return m_queue.size();
    }

    void enqueue(const_reference val)
    {
        ScopedLock lock(m_mutex);

        if (m_interrupted)
            return;

        m_queue.push(val);
        m_monitor.notify_one();
    }

    value_type dequeue()
    {
        ScopedLock lock(m_mutex);

        if (m_interrupted)
            return value_type();

        while (!m_interrupted && m_queue.empty())
            m_monitor.wait(lock);

        if (m_interrupted) // queue was interrupted
            return value_type();

        value_type val = m_queue.front();
        m_queue.pop();
        return val;
    }

    void interrupt()
    {
        ScopedLock lock(m_mutex);

        m_interrupted = true;
        m_monitor.notify_all();
    }

    void reset()
    {
        ScopedLock lock(m_mutex);

        m_interrupted = false;
    }

    bool isInterrupted()
    {
        ScopedLock lock(m_mutex);

        return m_interrupted;
    }

private:
    bool m_interrupted;
    boost::mutex m_mutex;
    boost::condition_variable m_monitor;
    std::queue<value_type> m_queue;
};

#endif // TSQUEUE_H


#pragma once

#include <Function.h>
#include <CircularBuffer.h>
#include <Mutex.h>

namespace vengine
{
    class WorkQueue
    {
    public:
        void enqueue(Function<void>&& task)
        {
            ScopedLock lock(m_mutex);
            m_buffer.enqueue(std::move(task));
        }

        Function<void> dequeue()
        {
            ScopedLock lock(m_mutex);
            return std::move(m_buffer.dequeue().value());
        }

    private:
        CircularBuffer<Function<void>> m_buffer { 512 };
        HybridMutex m_mutex {};
    };

    class WorkManager
    {
    public:
        static WorkManager& instance()
        {
            static WorkManager s_instance {};
            return s_instance;
        }

        WorkQueue& task_queue()
        {
            return m_task_queue;
        }

        u32 worker_count()
        {
            return 8;
        }

    private:
        WorkQueue m_task_queue;
    };
}
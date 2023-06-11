/*
    Copyright (C) 2022 iori (shortanemoia@protonmail.com)
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <Function.h>
#include <CircularBuffer.h>
#include <Mutex.h>
#include <Thread.h>

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
        
        size_t tasks_available()
        {
            ScopedLock lock(m_mutex);
            return m_buffer.size();
        }
        
        void wait()
        {
            m_mutex.lock();
            m_mutex.unlock();
        }

    private:
        CircularBuffer<Function<void>> m_buffer { 1024 };
        neo::SpinlockMutex m_mutex {};
    };

    class WorkManager
    {
    public:
        WorkManager(Context& context, detail::ContextBadge) : m_context(context), m_threads()
        {
            auto thread  = Thread::create([this](){
                while(true)
                {
                    m_task_queue.wait();
                    while (m_task_queue.tasks_available())
                    {
                        m_task_queue.dequeue()();
                    }
                }
            });
            
            m_threads.append(std::move(thread.result()));
        }

        WorkQueue& task_queue()
        {
            return m_task_queue;
        }

        u32 worker_count()
        {
            return 8;
            //FIXME
        }

    private:
        WorkQueue m_task_queue;
        Context& m_context;
        Vector<RefPtr<Thread>> m_threads;
    };
}
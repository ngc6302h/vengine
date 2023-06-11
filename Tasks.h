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

#include <Vector.h>
#include <Aligned.h>
#include "WorkManager.h"
#include "Archetype.h"

namespace vengine
{
    class Task
    {
    public:
        virtual void schedule(Context&, WorkQueue&) = 0;

        template<ConvertibleTo<Task*>... Dependencies>
        void depends_on(Dependencies... dependencies)
        {
            (m_dependencies.append(dependencies), ...);
        }

        void submit(Context& context)
        {
            schedule(context, context.work_manager().task_queue());
        }

    protected:
        Vector<Task*> m_dependencies;
    };

    template<typename TTask>
    class SingleTask : public Task
    {
    public:
        void schedule(Context& context, WorkQueue& queue) override
        {
            for (Task* task : m_dependencies)
                task->schedule(context, queue);

            queue.enqueue([this]()
                { static_cast<TTask*>(this)->execute(); });
        }
    };

    template<typename TTask, typename... TComponents>
    class ParallelTask : public Task
    {
    public:
        void schedule(Context& context, WorkQueue& queue) override
        {
            for (Task* task : m_dependencies)
                task->schedule(context, queue);

            auto system_execute_helper = [this](u64 iteration_start, u64 count, Aligned<TComponents*, 64>... components)
            {
                for (u64 i = iteration_start; i < count; ++i)
                {
                    static_cast<TTask*>(this)->execute(components[i]...);
                }
            };
            for (Archetype* archetype : m_archetypes)
            {
                const u64 chunk_size = Archetype::CHUNK_SIZE;
                for (size_t chunk = 0; chunk < archetype->size() / Archetype::CHUNK_SIZE; ++chunk)
                {
                    u64 strides;
                    u64 remaining_iterations;
                    u64 iterations_per_stride;
                    if (m_iterations_per_stride == 0)
                    {
                        strides = 1;
                        remaining_iterations = 0;
                        iterations_per_stride = chunk_size;
                    }
                    else
                    {
                        strides = chunk_size / m_iterations_per_stride;
                        remaining_iterations = chunk_size % m_iterations_per_stride;
                        iterations_per_stride = m_iterations_per_stride;
                    }

                    for (u64 s = 0; s < strides; ++s)
                    {
                        queue.enqueue([=, this]()
                            { system_execute_helper(s * iterations_per_stride, iterations_per_stride, archetype->template get_component_buffer<TComponents>(chunk)...); });
                    }
                    if (remaining_iterations > 0)
                    {
                        queue.enqueue([=, this]()
                            { system_execute_helper(strides * iterations_per_stride, remaining_iterations, archetype->template get_component_buffer<TComponents>(chunk)...); });
                    }
                }
                u64 remaining_iterations_for_this_archetype = archetype->size() % chunk_size;
                u64 strides;
                u64 iterations_per_stride;
                u64 remaining_iterations;
                if (m_iterations_per_stride == 0)
                {
                    strides = 1;
                    iterations_per_stride = remaining_iterations_for_this_archetype;
                    remaining_iterations = 0;
                }
                else
                {
                    strides = remaining_iterations_for_this_archetype / m_iterations_per_stride;
                    remaining_iterations = remaining_iterations_for_this_archetype % m_iterations_per_stride;
                    iterations_per_stride = m_iterations_per_stride;
                }

                for (u64 s = 0; s < strides; ++s)
                {
                    queue.enqueue([=, this]()
                        { system_execute_helper(s * iterations_per_stride, iterations_per_stride, archetype->template get_component_buffer<TComponents>(archetype->size() / chunk_size)...); });
                }
                if (remaining_iterations > 0)
                {
                    queue.enqueue([=, this]()
                        { system_execute_helper(strides * iterations_per_stride, remaining_iterations, archetype->template get_component_buffer<TComponents>(archetype->size() / chunk_size)...); });
                }
            }
        }

    private:
        Vector<Archetype*> m_archetypes;
        u64 m_iterations_per_stride { 0 };
    };

    template<typename TTask, typename... TComponents>
    class ParallelTaskWithIndex : public Task
    {
    public:
        void schedule(Context& context, WorkQueue& queue) override
        {
            for (Task* task : m_dependencies)
                task->schedule(context, queue);
            auto system_execute_helper = [this](u64 iteration_start, u64 count, Aligned<TComponents*, 64>... components)
            {
                for (u64 i = iteration_start; i < count; ++i)
                {
                    static_cast<TTask*>(this)->execute(i, components[i]...);
                }
            };
            for (Archetype* archetype : m_archetypes)
            {
                const u64 chunk_size = Archetype::CHUNK_SIZE;
                for (size_t chunk = 0; chunk < archetype->size() / Archetype::CHUNK_SIZE; ++chunk)
                {
                    u64 strides;
                    u64 remaining_iterations;
                    u64 iterations_per_stride;
                    if (m_iterations_per_stride == 0)
                    {
                        strides = 1;
                        remaining_iterations = 0;
                        iterations_per_stride = chunk_size;
                    }
                    else
                    {
                        strides = chunk_size / m_iterations_per_stride;
                        remaining_iterations = chunk_size % m_iterations_per_stride;
                        iterations_per_stride = m_iterations_per_stride;
                    }

                    for (u64 s = 0; s < strides; ++s)
                    {
                        queue.enqueue([=, this]()
                            { system_execute_helper(s * iterations_per_stride, iterations_per_stride, archetype->template get_component_buffer<TComponents>(chunk)...); });
                    }
                    if (remaining_iterations > 0)
                    {
                        queue.enqueue([=, this]()
                            { system_execute_helper(strides * iterations_per_stride, remaining_iterations, archetype->template get_component_buffer<TComponents>(chunk)...); });
                    }
                }
                u64 remaining_iterations_for_this_archetype = archetype->size() % chunk_size;
                u64 strides;
                u64 iterations_per_stride;
                u64 remaining_iterations;
                if (m_iterations_per_stride == 0)
                {
                    strides = 1;
                    iterations_per_stride = remaining_iterations_for_this_archetype;
                    remaining_iterations = 0;
                }
                else
                {
                    strides = remaining_iterations_for_this_archetype / m_iterations_per_stride;
                    remaining_iterations = remaining_iterations_for_this_archetype % m_iterations_per_stride;
                    iterations_per_stride = m_iterations_per_stride;
                }

                for (u64 s = 0; s < strides; ++s)
                {
                    queue.enqueue([=, this]()
                        { system_execute_helper(s * iterations_per_stride, iterations_per_stride, archetype->template get_component_buffer<TComponents>(archetype->size() / chunk_size)...); });
                }
                if (remaining_iterations > 0)
                {
                    queue.enqueue([=, this]()
                        { system_execute_helper(strides * iterations_per_stride, remaining_iterations, archetype->template get_component_buffer<TComponents>(archetype->size() / chunk_size)...); });
                }
            }
        }

    private:
        Vector<Archetype*> m_archetypes;
        u64 m_iterations_per_stride {0 };
    };

    template<typename TTask>
    class CustomParallelTask : public Task
    {
    public:
        void schedule(Context& context, WorkQueue& queue) override
        {
            for (Task* task : m_dependencies)
                task->schedule(context, queue);

            u64 iterations_per_stride = m_iterations / m_strides;
            u64 remaining_iterations = m_iterations % m_strides;

            for (u64 i = 0; i < m_strides; ++i)
            {
                
                queue.enqueue([=, this]()
                    {
                    for(u64 c = i*iterations_per_stride; c < iterations_per_stride; ++c)
                        static_cast<TTask*>(this)->execute(c); });
            }
            if (remaining_iterations > 0)
            {
                queue.enqueue([=, this]()
                    {
                    for(u64 c = iterations_per_stride * m_strides; c < remaining_iterations; ++c)
                        static_cast<TTask*>(this)->execute(c); });
            }
        }

    private:
        u64 m_iterations { 0 };
        u32 m_strides {1 };
    };
}
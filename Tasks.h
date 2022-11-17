
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
        virtual void schedule(WorkQueue&) = 0;

        template<ConvertibleTo<Task*>... Dependencies>
        void depends_on(Dependencies... dependencies)
        {
            (m_dependencies.append(dependencies), ...);
        }

        void submit()
        {
            schedule(WorkManager::instance().task_queue());
        }

    protected:
        Vector<Task*> m_dependencies;
    };

    template<typename TTask>
    class SingleTask : public Task
    {
    public:
        void schedule(WorkQueue& queue) override
        {
            for (Task* task : m_dependencies)
                task->schedule(queue);

            queue.enqueue([this]()
                { static_cast<TTask*>(this)->execute(); });
        }
    };

    template<typename TTask, typename... TComponents>
    class ParallelTask : public Task
    {
    public:
        void schedule(WorkQueue& queue) override
        {
            for (Task* task : m_dependencies)
                task->schedule(queue);

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
                    u64 splits;
                    u64 remaining_iterations;
                    u64 iterations_per_split;
                    if (m_iterations_per_split == 0)
                    {
                        splits = 1;
                        remaining_iterations = 0;
                        iterations_per_split = chunk_size;
                    }
                    else
                    {
                        splits = chunk_size / m_iterations_per_split;
                        remaining_iterations = chunk_size % m_iterations_per_split;
                        iterations_per_split = m_iterations_per_split;
                    }

                    for (u64 s = 0; s < splits; ++s)
                    {
                        queue.enqueue([=, this]()
                            { system_execute_helper(s * iterations_per_split, iterations_per_split, archetype->template get_component_buffer<TComponents>(chunk)...); });
                    }
                    if (remaining_iterations > 0)
                    {
                        queue.enqueue([=, this]()
                            { system_execute_helper(splits * iterations_per_split, remaining_iterations, archetype->template get_component_buffer<TComponents>(chunk)...); });
                    }
                }
                u64 remaining_iterations_for_this_archetype = archetype->size() % chunk_size;
                u64 splits;
                u64 iterations_per_split;
                u64 remaining_iterations;
                if (m_iterations_per_split == 0)
                {
                    splits = 1;
                    iterations_per_split = remaining_iterations_for_this_archetype;
                    remaining_iterations = 0;
                }
                else
                {
                    splits = remaining_iterations_for_this_archetype / m_iterations_per_split;
                    remaining_iterations = remaining_iterations_for_this_archetype % m_iterations_per_split;
                    iterations_per_split = m_iterations_per_split;
                }

                for (u64 s = 0; s < splits; ++s)
                {
                    queue.enqueue([=, this]()
                        { system_execute_helper(s * iterations_per_split, iterations_per_split, archetype->template get_component_buffer<TComponents>(archetype->size() / chunk_size)...); });
                }
                if (remaining_iterations > 0)
                {
                    queue.enqueue([=, this]()
                        { system_execute_helper(splits * iterations_per_split, remaining_iterations, archetype->template get_component_buffer<TComponents>(archetype->size() / chunk_size)...); });
                }
            }
        }

        void set_iterations_per_split(u64 value)
        {
            m_iterations_per_split = value;
        }

    private:
        Vector<Archetype*> m_archetypes;
        u64 m_iterations_per_split { 0 };
    };

    template<typename TTask, typename... TComponents>
    class ParallelTaskWithIndex : public Task
    {
    public:
        void schedule(WorkQueue& queue) override
        {
            for (Task* task : m_dependencies)
                task->schedule(queue);
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
                    u64 splits;
                    u64 remaining_iterations;
                    u64 iterations_per_split;
                    if (m_iterations_per_split == 0)
                    {
                        splits = 1;
                        remaining_iterations = 0;
                        iterations_per_split = chunk_size;
                    }
                    else
                    {
                        splits = chunk_size / m_iterations_per_split;
                        remaining_iterations = chunk_size % m_iterations_per_split;
                        iterations_per_split = m_iterations_per_split;
                    }

                    for (u64 s = 0; s < splits; ++s)
                    {
                        queue.enqueue([=, this]()
                            { system_execute_helper(s * iterations_per_split, iterations_per_split, archetype->template get_component_buffer<TComponents>(chunk)...); });
                    }
                    if (remaining_iterations > 0)
                    {
                        queue.enqueue([=, this]()
                            { system_execute_helper(splits * iterations_per_split, remaining_iterations, archetype->template get_component_buffer<TComponents>(chunk)...); });
                    }
                }
                u64 remaining_iterations_for_this_archetype = archetype->size() % chunk_size;
                u64 splits;
                u64 iterations_per_split;
                u64 remaining_iterations;
                if (m_iterations_per_split == 0)
                {
                    splits = 1;
                    iterations_per_split = remaining_iterations_for_this_archetype;
                    remaining_iterations = 0;
                }
                else
                {
                    splits = remaining_iterations_for_this_archetype / m_iterations_per_split;
                    remaining_iterations = remaining_iterations_for_this_archetype % m_iterations_per_split;
                    iterations_per_split = m_iterations_per_split;
                }

                for (u64 s = 0; s < splits; ++s)
                {
                    queue.enqueue([=, this]()
                        { system_execute_helper(s * iterations_per_split, iterations_per_split, archetype->template get_component_buffer<TComponents>(archetype->size() / chunk_size)...); });
                }
                if (remaining_iterations > 0)
                {
                    queue.enqueue([=, this]()
                        { system_execute_helper(splits * iterations_per_split, remaining_iterations, archetype->template get_component_buffer<TComponents>(archetype->size() / chunk_size)...); });
                }
            }
        }

        void set_iterations_per_split(u64 value)
        {
            m_iterations_per_split = value;
        }

    private:
        Vector<Archetype*> m_archetypes;
        u64 m_iterations_per_split { 0 };
    };

    template<typename TTask>
    class CustomParallelTask : public Task
    {
    public:
        void schedule(WorkQueue& queue) override
        {
            for (Task* task : m_dependencies)
                task->schedule(queue);

            u64 iterations_per_split = m_iterations / m_splits;
            u64 remaining_iterations = m_iterations % m_splits;

            for (u64 i = 0; i < m_splits; ++i)
            {
                queue.enqueue([=, this]()
                    {
                    for(u64 c = i*iterations_per_split; c < iterations_per_split; ++c)
                        static_cast<TTask*>(this)->execute(c); });
            }
            if (remaining_iterations > 0)
            {
                queue.enqueue([=, this]()
                    {
                    for(u64 c = iterations_per_split*m_splits; c < remaining_iterations; ++c)
                        static_cast<TTask*>(this)->execute(c); });
            }
        }

        void set_iterations(u64 value)
        {
            m_iterations = value;
        }

        void set_splits(u64 value)
        {
            if (value == 0)
                m_splits = WorkManager::instance().worker_count();
            else
                m_splits = value;
        }

    private:
        u64 m_iterations { 0 };
        u32 m_splits { 1 };
    };
}
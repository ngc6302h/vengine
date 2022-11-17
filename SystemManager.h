
#pragma once

#include <Vector.h>
#include <Memory.h>
#include <SmartPtr.h>
#include "System.h"
#include "Context.h"

namespace vengine
{
    class SimulationUpdateStartSystem : public System
    {
        void on_update() override
        {
            __builtin_printf("Simulation %lu\n", m_counter);
        }

        u64 m_counter;
    };

    struct SystemList
    {
        SystemList* next { nullptr };
        SystemList* prev { nullptr };
        System* system { nullptr };

        SystemList* find(System* sys)
        {
            if (system == sys)
                return this;
            else if (next != nullptr)
                return next->find(sys);
            else
                return nullptr;
        }

        SystemList* last()
        {
            if (next == nullptr)
                return this;
            else
                return next->last();
        }

        void insert_next(System* sys)
        {
            SystemList* new_node = create<SystemList>(next, this, sys).release_nonnull().release();
            next = new_node;
        }

        void remove(System* sys)
        {
            if (system == sys)
            {
                if (prev != nullptr)
                {
                    prev->next = next;
                }
                if (next != nullptr)
                {
                    next->prev = prev;
                }
            }
            else if (next != nullptr)
                next->remove(sys);
        }
    };

    class SystemManager
    {
    public:
        SystemManager() = delete;
        SystemManager& operator=(SystemManager&&) = delete;
        SystemManager& operator=(SystemManager const&) = delete;
        explicit SystemManager(Context& context, detail::ContextBadge) :
            m_context(context),
            m_first(create<SystemList>(nullptr, nullptr, create<SimulationUpdateStartSystem>().release_nonnull().release()).release_nonnull().release())
        {
        }

        void register_system(System* system, System* execute_before = nullptr)
        {
            SystemList* node = execute_before != nullptr ? m_first->find(execute_before) : m_first->last();

            node->insert_next(system);
            system->on_register();
        }

        void unregister_system(System* system)
        {
            system->on_deregister();
            m_first->remove(system);
        }

    private:
        Context& m_context;
        SystemList* m_first;
    };
}
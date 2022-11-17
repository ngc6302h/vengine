//
// Created by ngc on 16/11/22.
//

#pragma once

#include <Memory.h>
#include "Badges.h"

namespace vengine
{
    class EntityManager;
    class ArchetypeManager;
    class SystemManager;

    class Context
    {
    public:
        Context();
        EntityManager& entity_manager();
        SystemManager& system_manager();
        ArchetypeManager& archetype_manager();

    private:
        OwnPtr<EntityManager> m_entity_manager;
        OwnPtr<SystemManager> m_system_manager;
        OwnPtr<ArchetypeManager> m_archetype_manager;
    };
}
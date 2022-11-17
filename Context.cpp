//
// Created by ngc on 17/11/22.
//

#include "Context.h"
#include "EntityManager.h"
#include "Archetype.h"
#include "SystemManager.h"

namespace vengine
{

    Context::Context() :
        m_entity_manager(create<EntityManager>(*this, detail::ContextBadge {}).release_nonnull()),
        m_system_manager(create<SystemManager>(*this, detail::ContextBadge {}).release_nonnull()),
        m_archetype_manager(create<ArchetypeManager>(*this, detail::ContextBadge {}).release_nonnull())
    {
    }

    EntityManager& Context::entity_manager()
    {
        return *m_entity_manager;
    }

    SystemManager& Context::system_manager()
    {
        return *m_system_manager;
    }

    ArchetypeManager& Context::archetype_manager()
    {
        return *m_archetype_manager;
    }
}
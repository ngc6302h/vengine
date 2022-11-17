//
// Created by ngc on 6/11/22.
//

#include "EntityManager.h"

vengine::EntityManager::EntityManager(vengine::Context& context, vengine::detail::ContextBadge) :
    m_context(context) { }

vengine::StableEntityID vengine::EntityManager::get_stable_entity_reference(vengine::EntityID entity)
{
    StableEntityID ref(entity);
    m_context.archetype_manager()
        .get_archetype_by_id(GET_ARCHETYPE_ID_FROM_ENTITY_ID(entity))
        ->stable_entity_references()
        .append(ref);

    return ref;
}

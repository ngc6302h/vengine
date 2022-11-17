//
// Created by ngc on 5/11/22.
//

#include "Archetype.h"
#include "Debug.h"

namespace vengine
{
    Archetype::Archetype(Vector<Type const*> const& component_types) :
        m_types(), m_entities(type_of<EntityID>(), (u64)4, CHUNK_SIZE)
    {
        for (auto type : component_types)
        {
            m_types.append(type);
            m_components.construct(type, (u64)4, CHUNK_SIZE);
        }
        static Atomic<u64> next_id { 1 };
        m_id = next_id.fetch_add(1, MemoryOrder::Relaxed);
    }

    bool Archetype::has_type(Type const* type)
    {
        return m_components
            .to_iterable_collection()
            .contains(type, [](ChunkedBuffer<u8> const& a, Type const* b)
                { return a.type() == b; });
    }

    void Archetype::set_component_data(EntityID entity, Type const* component_type, u8 const* data)
    {
        ENSURE(GET_ARCHETYPE_ID_FROM_ENTITY_ID(entity) == m_id);
        auto& buffer = *m_components.to_iterable_collection()
                            .find(component_type, [](ChunkedBuffer<u8> const& buffer, Type const* type)
                                { return buffer.type() == type; });
        component_type->copy_assignment(data, buffer[GET_INDEX_FROM_ENTITY_ID(entity)]);
    }

    void Archetype::destroy(EntityID entity)
    {
        ENSURE(GET_ARCHETYPE_ID_FROM_ENTITY_ID(entity) == m_id);
        auto index = GET_INDEX_FROM_ENTITY_ID(entity);
        if (index >= size())
        {
            if constexpr (DebugMessages)
                __builtin_printf("Tried to destroy nonexistent entity %lx!!!\n", entity);
            return;
        }
        for (auto& buffer : m_components)
        {
            buffer.remove_at(index);
        }
        m_entities.remove_at(index);

        if (m_entities.size() > 0 && index != m_entities.size())
        {
            auto& moved_entity = m_entities.at(index);
            auto old_entity = moved_entity;
            moved_entity = MAKE_ENTITY_ID(m_id, index);

            auto maybe_iterator = m_stable_references.to_iterable_collection().find(old_entity, [](StableEntityID const& stableId, EntityID id)
                { return stableId.id() == id; });
            if (!maybe_iterator.is_end())
                maybe_iterator->update(moved_entity);
        }

        auto maybe_iterator = m_stable_references.to_iterable_collection().find(entity, [](StableEntityID const& stableId, EntityID id)
            { return stableId.id() == id; });
        if (!maybe_iterator.is_end())
        {
            maybe_iterator->update(0);
            m_stable_references.remove_at(maybe_iterator.index());
        }
    }

    u8* Archetype::get_component_data(EntityID id, Type const* type)
    {
        ENSURE(GET_ARCHETYPE_ID_FROM_ENTITY_ID(id) == m_id);
        return m_components
            .to_iterable_collection()
            .find(type, [](ChunkedBuffer<u8> const& buffer, Type const* type)
                { return buffer.type() == type; })
            ->
            operator[](GET_INDEX_FROM_ENTITY_ID(id));
    }

    u64 Archetype::id() const
    {
        return m_id;
    }

    size_t Archetype::size() const
    {
        return m_entities.size();
    }

    Vector<Type const*> const& Archetype::component_types() const
    {
        return m_types;
    }

    Vector<StableEntityID>& Archetype::stable_entity_references()
    {
        return m_stable_references;
    }

    size_t Archetype::index_of_type(Type const* type)
    {
        for (size_t i = 0; i < m_types.size(); ++i)
        {
            if (type == m_types[i])
                return i;
        }

        VERIFY_NOT_REACHED();
    }
}
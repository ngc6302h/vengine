//
// Created by ngc on 5/11/22.
//

#pragma once
#include <Vector.h>
#include <Buffer.h>
#include <Atomic.h>
#include <Hashmap.h>
#include "RTTI.h"
#include "Entity.h"
#include "ChunkedBuffer.h"
#include "Badges.h"

namespace vengine
{
    class Archetype
    {
    public:
        static constexpr size_t CHUNK_SIZE = 4094;

        explicit Archetype(Vector<Type const*> const& component_types);
        bool has_type(Type const* type);

    private:
        template<typename TComponent>
        void set_component_data_at_index(size_t index, TComponent const& data)
        {
            auto& buffer = m_components[index_of_type(type_of<TComponent>())];
            if (index < m_entities.size())
                buffer.template at<TComponent>(index) = data;
            else
                buffer.append(data);
        }

        void set_component_data_at_index(size_t index, Type const* type, u8 const* data)
        {
            auto& buffer = m_components[index_of_type(type)];
            if (index < m_entities.size())
                type->copy_assignment(data, buffer[index]);
            else
                buffer.append(data);
        }

    public:
        void set_component_data(EntityID entity, Type const* component_type, u8 const* data);

        template<typename TComponent>
        void set_component_data(EntityID entity, TComponent const& data)
        {
            set_component_data_at_index(GET_INDEX_FROM_ENTITY_ID(entity), data);
        }

        template<typename... TComponents>
        EntityID create(TComponents const&... components)
        {
            EntityID entity = MAKE_ENTITY_ID(m_id, m_entities.size());
            (set_component_data_at_index(m_entities.size(), components), ...);
            m_entities.append(entity);
            return entity;
        }

        EntityID create(Vector<Tuple<Type const*, u8*>> const& components)
        {
            EntityID entity = MAKE_ENTITY_ID(m_id, m_entities.size());
            for (auto [type, data] : components)
                set_component_data_at_index(m_entities.size(), type, data);
            m_entities.append(entity);
            return entity;
        }

        void destroy(EntityID entity);
        u8* get_component_data(EntityID id, Type const* type);

        template<typename T>
        T* get_component_buffer(size_t chunk_index)
        {
            return (T*)m_components[index_of_type(type_of<T>())].get_buffer_data(chunk_index);
        }

        size_t index_of_type(Type const* type);

        u64 id() const;
        size_t size() const;
        Vector<Type const*> const& component_types() const;
        Vector<StableEntityID>& stable_entity_references();

    private:
        u64 m_id;
        ChunkedBuffer<EntityID> m_entities;
        Vector<StableEntityID> m_stable_references;
        Vector<ChunkedBuffer<u8>> m_components;
        Vector<Type const*> m_types;
    };

    class ArchetypeManager
    {
    public:
        ArchetypeManager() = delete;
        ArchetypeManager& operator=(ArchetypeManager&&) = delete;
        ArchetypeManager& operator=(ArchetypeManager const&) = delete;

        explicit ArchetypeManager(Context& context, detail::ContextBadge) :
            m_context(context), m_archetypes(), m_archetype_map(16, 64) {};

        Archetype* get_or_create_archetype(Vector<Type const*> const& component_types)
        {
            for (auto& archetype : m_archetypes)
            {
                bool found = true;
                for (auto type : component_types)
                {
                    if (!archetype->has_type(type))
                    {
                        found = false;
                        break;
                    }
                }
                if (found)
                    return archetype;
            }

            auto new_archetype = new Archetype(component_types);
            m_archetypes.append(new_archetype);
            m_archetype_map.insert(new_archetype->id(), new_archetype);
            return new_archetype;
        }

        Vector<Archetype*>& archetypes()
        {
            return m_archetypes;
        }

        Archetype* get_archetype_by_id(u32 id)
        {
            auto archetype = m_archetype_map.get(id);
            ENSURE(archetype.has_value());
            return archetype.value();
        }

    private:
        Context& m_context;
        Vector<Archetype*> m_archetypes;
        Hashmap<u32, Archetype*> m_archetype_map;
    };

}
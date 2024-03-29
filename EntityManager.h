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
#include "Entity.h"
#include "Archetype.h"
#include "RTTI.h"
#include "Badges.h"
#include "Context.h"

namespace vengine
{

    class EntityManager
    {
    public:
        EntityManager() = delete;
        EntityManager& operator=(EntityManager&&) = delete;
        EntityManager& operator=(EntityManager const) = delete;

        explicit EntityManager(Context& context, detail::ContextBadge);
        ;
        StableEntityID get_stable_entity_reference(EntityID entity);

        template<typename... TComponents>
        EntityID create(TComponents const&... components)
        {
            Vector<Type const*> component_types { type_of<TComponents>()... };
            sort(component_types, [](Type const* a, Type const* b)
                { return a->id() < b->id(); });

            auto* archetype = m_context.archetype_manager().get_or_create_archetype(component_types);

            EntityID id = archetype->template create(components...);
            return id;
        }

        void destroy(EntityID entity)
        {
            auto archetype = m_context.archetype_manager().get_archetype_by_id(GET_ARCHETYPE_ID_FROM_ENTITY_ID(entity));
            archetype->destroy(entity);
        }

        template<typename TComponent>
        Optional<EntityID> add_component(EntityID entity, TComponent const& data)
        {
            auto* archetype = m_context.archetype_manager().get_archetype_by_id(GET_ARCHETYPE_ID_FROM_ENTITY_ID(entity));
            if (archetype->has_type(type_of<TComponent>()))
                return {};

            auto new_component_list = archetype->component_types();
            new_component_list.append(type_of<TComponent>());
            sort(new_component_list, [](Type const* a, Type const* b)
                { return a->id() < b->id(); });

            auto* new_archetype = m_context.archetype_manager().get_or_create_archetype(new_component_list);

            Vector<Tuple<Type const*, u8*>> new_data;
            for (auto const& component_type : archetype->component_types())
                new_data.append(make_tuple(component_type, archetype->get_component_data(entity, component_type)));
            new_data.append(make_tuple(type_of<TComponent>(), (u8*)&data));

            auto new_entity_id = new_archetype->template create(new_data);

            auto maybe_stable_reference = archetype->stable_entity_references().to_iterable_collection().find(entity, [](StableEntityID const& stableId, EntityID id)
                { return stableId.id() == id; });
            if (!maybe_stable_reference.is_end())
            {
                maybe_stable_reference->update(new_entity_id);
                new_archetype->stable_entity_references().append(*maybe_stable_reference);
                archetype->stable_entity_references().remove_at(maybe_stable_reference.index());
            }

            archetype->destroy(entity);

            return new_entity_id;
        }

    private:
        Context& m_context;
    };
}

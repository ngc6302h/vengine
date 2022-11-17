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
#include <Types.h>
#include <Memory.h>

// EntityID(64 bits) [63-44: archetype id][43-0: entity id]
// Archetype ID represents the unique id of the archetype that owns this entity.
// Entity ID represents the index of this entity in the archetype buffers.
// Every whole EntityID value is unique
// Adding or removing components changes both fields, so entity ids are not persistent.
// Use StableEntityID if you want a stable reference to an entity

#define GET_INDEX_FROM_ENTITY_ID(x) ((x)&0xFFFFFFFFFFF)
#define GET_ARCHETYPE_ID_FROM_ENTITY_ID(x) ((x) >> 44)
#define MAKE_ENTITY_ID(archetype_id, index) (((archetype_id) << 44) | (index))
namespace vengine
{
    using EntityID = IdentityType<u64>;
    extern u64 get_next_entity_id(u64 archetype_id);

    class StableEntityID
    {
        friend class Archetype;
        friend class EntityManager;

    public:
        EntityID id() const
        {
            return *m_current_id;
        }
        StableEntityID() = delete;

    private:

        explicit StableEntityID(EntityID id) :
            m_current_id(create_refcounted<EntityID>(id).release_nonnull())
        {
        }

        void update(EntityID new_id)
        {
            *m_current_id = new_id;
        }

        RefPtr<EntityID> m_current_id;
    };
}
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

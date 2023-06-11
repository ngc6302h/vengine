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

#include "Context.h"
#include "EntityManager.h"
#include "Archetype.h"
#include "SystemManager.h"
#include "WorkManager.h"

namespace vengine
{
    Context::Context(SubsystemData&& subsystems) :  m_entity_manager(create<EntityManager>(*this, detail::ContextBadge {}).release_nonnull()),
                                                            m_system_manager(create<SystemManager>(*this, detail::ContextBadge {}).release_nonnull()),
                                                            m_archetype_manager(create<ArchetypeManager>(*this, detail::ContextBadge {}).release_nonnull()),
                                                            m_work_manager(create<WorkManager>(*this, detail::ContextBadge {}).release_nonnull()),
                                                            m_input(std::move(subsystems.input_subsystem)),
                                                            m_window(std::move(subsystems.window_subsystem))
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
    
    Input& Context::input()
    {
        return *m_input;
    }
    
    Window &Context::window()
    {
        return *m_window;
    }
    
    WorkManager& Context::work_manager()
    {
        return *m_work_manager;
    }
}
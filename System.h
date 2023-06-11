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

#include "Archetype.h"

namespace vengine
{

    class System
    {
        friend class SystemManager;

    public:
        System(String name, Context& context) : m_name(std::move(name)), m_context(context)
        {}
    
        virtual ~System() = default;

        String& name()
        {
            return m_name;
        }
    
        String const& name() const
        {
            return m_name;
        }
    
        Context& context()
        {
            return m_context;
        }
    
        Context const& context() const
        {
            return m_context;
        }
        
    protected:
    public:
        virtual void on_update() {};
        virtual void on_register() { }
        virtual void on_deregister() { }
        
    protected:
        String m_name;
        Context& m_context;
    };
}
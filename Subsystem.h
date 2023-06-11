/*
    Copyright (C) 2022-2023 iori (shortanemoia@protonmail.com)
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

#include "Window.h"
#include "Input.h"
#include <SmartPtr.h>

namespace vengine
{
    struct SubsystemData
    {
        OwnPtr<Window> window_subsystem;
        OwnPtr<Input> input_subsystem;
    };
    
    class SubsystemCreationError
    {
    public:
    
        SubsystemCreationError(String&& message) : m_message(std::move(message))
        {}
        
        String const& message()  const
        {
            return m_message;
        }
    private:
        String m_message;
    };
}
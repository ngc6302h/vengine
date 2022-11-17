/*
 * Copyright (C) 2021  César Torres (shortanemoia@protonmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <TypeTraits.h>
#include "RTTI.h"

namespace ngx::rtti
{
    size_t get_next_type_id()
    {
        static size_t next_id = 0;
        return next_id++;
    }

    String const& Type::name() const
    {
        return m_name;
    }

    size_t Type::size() const
    {
        return m_size;
    }

    size_t Type::alignment() const
    {
        return m_alignment;
    }

    TypeID Type::id() const
    {
        return m_id;
    }

    bool Type::is_trivially_copyable() const
    {
        return m_is_trivially_copyable;
    }

    bool Type::is_trivially_destructible() const
    {
        return m_is_trivially_destructible;
    }

    void Type::copy_assignment(void const* from, void* to) const
    {
        m_copy_assignment(from, to);
    }

    void Type::copy_assignment(size_t num, void const* from, void* to) const
    {
        m_copy_assignment_many(num, from, to);
    }

    void Type::move_assignment(void* from, void* to) const
    {
        m_move_assignment(from, to);
    }

    void Type::move_assignment(size_t num, void* from, void* to) const
    {
        m_move_assignment_many(num, from, to);
    }

    void Type::destructor(void* ptr) const
    {
        m_destructor(ptr);
    }
}
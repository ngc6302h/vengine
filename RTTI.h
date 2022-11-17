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
#include <TypeExtras.h>

#pragma once

namespace ngx::rtti
{
    using TypeID = u64;

    extern size_t get_next_type_id();

    class Type
    {
        template<typename T>
        friend Type const* type_of();

    public:
        ~Type()
        {
        }

        String const& name() const;
        size_t size() const;
        size_t alignment() const;
        TypeID id() const;
        bool is_trivially_copyable() const;
        bool is_trivially_destructible() const;
        void copy_assignment(void const* from, void* to) const;
        void move_assignment(void* from, void* to) const;
        void move_assignment(size_t num, void* from, void* to) const;
        void destructor(void* ptr) const;
        void copy_assignment(size_t num, void const* from, void* to) const;

        template<typename T>
        void copy_assignment(T const& from, T& to) const
        {
            to = from;
        }

        template<typename T>
        void move_assignment(T& from, T& to) const
        {
            to = std::move(from);
        }

    private:
        Type() = default;
        Type& operator=(Type const&) = default;

        template<typename T>
        static Type create()
        {
            Type new_type_info;
            new_type_info.m_name = nameof<T>;
            new_type_info.m_size = sizeof(T);
            new_type_info.m_alignment = alignof(T);
            new_type_info.m_id = get_next_type_id();
            new_type_info.m_is_trivially_copyable = neo::IsTriviallyCopyable<T>;
            new_type_info.m_is_trivially_destructible = neo::IsTriviallyDestructible<T>;
            new_type_info.m_copy_assignment = [](void const* from, void* to) -> void
            { *reinterpret_cast<T*>(to) = *reinterpret_cast<T const*>(from); };
            new_type_info.m_copy_assignment_many = [](size_t num, void const* from, void* to) -> void
            { for (size_t i = 0; i < num; ++i) *(reinterpret_cast<T*>(to)+i) = std::move(*(reinterpret_cast<T const*>(from)+i)); };
            new_type_info.m_move_assignment = [](void const* from, void* to) -> void
            { *reinterpret_cast<T*>(to) = std::move(*reinterpret_cast<T const*>(from)); };
            new_type_info.m_move_assignment_many = [](size_t num, void const* from, void* to) -> void
            { for (size_t i = 0; i < num; ++i) *(reinterpret_cast<T*>(to)+i) = std::move(*(reinterpret_cast<T const*>(from)+i)); };
            new_type_info.m_destructor = [](void* ptr)
            { reinterpret_cast<T*>(ptr)->~T(); };
            return new_type_info;
        }

        String m_name;
        size_t m_size;
        size_t m_alignment;
        TypeID m_id;
        bool m_is_trivially_copyable;
        bool m_is_trivially_destructible;
        void (*m_copy_assignment)(void const*, void*);
        void (*m_copy_assignment_many)(size_t, void const*, void*);
        void (*m_move_assignment)(void const*, void*);
        void (*m_move_assignment_many)(size_t, void const*, void*);
        void (*m_destructor)(void*);
    };

    template<typename T>
    Type const* type_of()
    {
        static const Type type = Type::create<T>();
        return &type;
    }

    template<typename...>
    struct Pack
    {
    };
}
using ngx::rtti::Pack;
using ngx::rtti::Type;
using ngx::rtti::type_of;
using ngx::rtti::TypeID;

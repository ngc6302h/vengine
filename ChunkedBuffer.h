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

#include <Vector.h>
#include <Buffer.h>
#include "RTTI.h"

namespace vengine
{
    template<typename T = u8>
    class ChunkedBuffer
    {
    public:
        static constexpr size_t DATA_ALIGNMENT = 64;
        ChunkedBuffer(Type const* type, u64 max_unused_buffers, u64 chunk_size) :
            m_buffers(), m_type(type), m_max_unused_buffers(max_unused_buffers), m_chunk_size(chunk_size), m_size() { }
        ~ChunkedBuffer()
        {
        }
        u8* operator[](u64 index)
        {
            return (u8*)&m_buffers[index / m_chunk_size][(index % m_chunk_size) * m_type->size()];
        }

        template<typename K = T>
        K& at(u64 index)
        {
            return *(K*)(*this)[index];
        }

        void remove_at(u64 index)
        {
            if (index == m_size - 1)
            {
                m_size--;
                if (!m_type->is_trivially_destructible())
                    m_type->destructor((*this)[m_size]);
                return;
            }

            m_type->move_assignment((*this)[m_size - 1], (*this)[index]);

            auto unused_buffers = m_buffers.size() - m_size / m_chunk_size;
            while (unused_buffers-- > m_max_unused_buffers)
                m_buffers.take_last();
            m_size--;
        }

        void append(u8 const* data)
        {
            if (m_size % m_chunk_size == 0)
            {
                Optional<Buffer<T>> buffer = Buffer<T>::create_uninitialized(m_chunk_size * m_type->size(), DATA_ALIGNMENT);
                ENSURE(buffer.has_value());
                m_buffers.append(std::move(buffer.value()));
            }

            m_type->move_assignment((void*)data, (*this)[m_size]);
            m_size++;
        }

        template<typename K>
        void append(K const& element)
        {
            if (m_size % m_chunk_size == 0)
            {
                Optional<Buffer<T>> buffer = Buffer<T>::create_uninitialized(m_chunk_size * m_type->size(), DATA_ALIGNMENT);
                ENSURE(buffer.has_value());
                m_buffers.append(std::move(buffer.value()));
            }

            at<K>(m_size) = element;
            m_size++;
        }

        void* get_buffer_data(size_t chunk_index)
        {
            return m_buffers[chunk_index].data();
        }

        u64 size() const
        {
            return m_size;
        }

        Type const* type() const
        {
            return m_type;
        }

    private:
        Vector<Buffer<T>> m_buffers;
        Type const* m_type;
        u64 m_max_unused_buffers;
        u64 m_chunk_size;
        u64 m_size;
    };
}
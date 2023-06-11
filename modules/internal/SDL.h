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
#include <SDL2/SDL.h>
#define NEO_DO_NOT_DEFINE_STD
#include <String.h>
#include "../../Input.h"
#include "../../Window.h"
#include "../SDLStructs.h"
namespace vengine
{
    class SDLInput : public Input
    {
    private:
        void update_inputs() override;
    public:
        virtual bool get_key_down(KeyboardKey key, KeyboardModifiers modifiers) override;
        virtual bool get_key_up(KeyboardKey key, KeyboardModifiers modifiers) override;
        virtual bool get_key(KeyboardKey key, KeyboardModifiers modifiers) override;
        virtual bool get_physical_key_down(KeyboardScancode scancode, KeyboardModifiers modifiers) override;
        virtual bool get_physical_key_up(KeyboardScancode scancode, KeyboardModifiers modifiers) override;
        virtual bool get_physical_key(KeyboardScancode scancode, KeyboardModifiers modifiers) override;
        virtual String get_key_name(KeyboardKey key) override;
        virtual String get_scancode_name(KeyboardScancode scancode) override;
    private:
        Vector<SDL_KeyboardEvent> m_keyboard_events;
        HybridMutex m_get_key_name_mutex;
        HybridMutex m_get_scancode_name_mutex;
    };
    
    class SDLWindow : public Window
    {
    private:
        SDLWindow(SDL_Window* window, String const& name, u32 width, u32 height, SDLWindowFlags flags);
        void close();
    public:
        static ResultOrError<OwnPtr<SDLWindow>, String> create(SDLWindowFlags flags);
        ~SDLWindow() override = default;
    private:
        SDL_Window* m_window;
        String m_title;
        u32 m_width;
        u32 m_height;
        SDLWindowFlags m_flags;
    };
}
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

#include "SDL.h"
#include "Mutex.h"
#define NEO_DO_NOT_DEFINE_STD
#include "String.h"

namespace vengine
{
    SDL_KeyCode key_to_sdl_key(KeyboardKey key)
    {
        switch (key)
        {
            case input::KeyboardKey::Key0:
                return SDLK_0;
            case input::KeyboardKey::Key1:
                return SDLK_1;
        }
        VERIFY_NOT_REACHED();
    }
    
    SDL_Scancode scancode_to_sdl_scancode(KeyboardScancode key)
    {
        switch (key)
        {
            case input::KeyboardScancode::Key0:
                return SDL_SCANCODE_0;
            case input::KeyboardScancode::Key1:
                return SDL_SCANCODE_1;
        }
        VERIFY_NOT_REACHED();
    }
    
    SDL_Keymod keymod_to_sdl_keymod(KeyboardModifiers mod)
    {
        return static_cast<SDL_Keymod>(0);
    }
    
    
    void SDLInput::update_inputs()
    {
        m_keyboard_events.clear();
        SDL_Event event {};
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                {
                    m_keyboard_events.append(event.key);
                }
            }
        }
    }
    
    bool SDLInput::get_key_down(KeyboardKey key, KeyboardModifiers modifiers)
    {
        auto k = key_to_sdl_key(key);
        auto mod = keymod_to_sdl_keymod(modifiers);
        for(auto const& event : m_keyboard_events)
        {
            if (event.type == SDL_KEYDOWN && event.keysym.sym == k && event.keysym.mod == mod && event.repeat == 0)
                return true;
        }
        return false;
    }
    
    bool SDLInput::get_key_up(KeyboardKey key, KeyboardModifiers modifiers)
    {
        auto k = key_to_sdl_key(key);
        auto mod = keymod_to_sdl_keymod(modifiers);
        for(auto const& event : m_keyboard_events)
        {
            if (event.type == SDL_KEYUP && event.keysym.sym == k && event.keysym.mod == mod && event.repeat == 0)
                return true;
        }
        return false;
    }
    
    bool SDLInput::get_key(KeyboardKey key, KeyboardModifiers modifiers)
    {
        auto k = key_to_sdl_key(key);
        auto mod = keymod_to_sdl_keymod(modifiers);
        for(auto const& event : m_keyboard_events)
        {
            if (event.type == SDL_KEYDOWN && event.keysym.sym == k && event.keysym.mod == mod && event.repeat == 1)
                return true;
        }
        return false;
    }
    
    bool SDLInput::get_physical_key_down(KeyboardScancode scancode, KeyboardModifiers modifiers)
    {
        auto mod = keymod_to_sdl_keymod(modifiers);
        auto code = scancode_to_sdl_scancode(scancode);
        for(auto const& event : m_keyboard_events)
        {
            if (event.type == SDL_KEYDOWN && event.keysym.scancode == code && event.keysym.mod == mod && event.repeat == 0)
                return true;
        }
        return false;
    }
    
    bool SDLInput::get_physical_key_up(KeyboardScancode scancode, KeyboardModifiers modifiers)
    {
        auto mod = keymod_to_sdl_keymod(modifiers);
        auto code = scancode_to_sdl_scancode(scancode);
        for(auto const& event : m_keyboard_events)
        {
            if (event.type == SDL_KEYUP && event.keysym.scancode == code && event.keysym.mod == mod && event.repeat == 0)
                return true;
        }
        return false;
    }
    
    bool SDLInput::get_physical_key(KeyboardScancode scancode, KeyboardModifiers modifiers)
    {
        auto mod = keymod_to_sdl_keymod(modifiers);
        auto code = scancode_to_sdl_scancode(scancode);
        for(auto const& event : m_keyboard_events)
        {
            if (event.type == SDL_KEYDOWN && event.keysym.scancode == code && event.keysym.mod == mod && event.repeat == 1)
                return true;
        }
        return false;
    }
    
    String SDLInput::get_key_name(KeyboardKey key)
    {
        auto k = key_to_sdl_key(key);
        ScopedLock lock(m_get_key_name_mutex);
        return String {SDL_GetKeyName(k)};
    }
    
    String SDLInput::get_scancode_name(KeyboardScancode scancode)
    {
        auto s = scancode_to_sdl_scancode(scancode);
        ScopedLock lock(m_get_scancode_name_mutex);
        return String {SDL_GetScancodeName(s)};
    }

    ResultOrError<OwnPtr<SDLWindow>, String> SDLWindow::create(SDLWindowFlags flags)
    {
        if ((flags & SDLWindowFlags::Vulkan) == SDLWindowFlags::Vulkan)
        {
            SDL_Window* window = SDL_CreateWindow("vengine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_VULKAN);
            auto maybe_object = neo::create<SDLWindow>(SDLWindow(window, "vengine", 800, 600, flags));
            if (maybe_object.leak_ptr())
                return maybe_object.release_nonnull();
        }
        return "Couldn't create window!"_s;
    }
    
    SDLWindow::SDLWindow(SDL_Window* window, String const &name, u32 width, u32 height, SDLWindowFlags flags) : m_flags(flags), m_height(height), m_width(width), m_window(window)
    {
    }
    
    void SDLWindow::close()
    {
        SDL_DestroyWindow(m_window);
    }
}
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
#define NEO_DO_NOT_DEFINE_STD
#include <Types.h>
#include <Mutex.h>
#include <String.h>
#undef NEO_DO_NOT_DEFINE_STD

namespace vengine
{
    namespace input
    {
        enum class KeyboardModifiers : u16
        {
            None = 1<<0,
            LeftShift = 1<<1,
            RightShift = 1<<2,
            LeftControl = 1<<3,
            RightControl = 1<< 4,
            LeftAlt = 1<<5,
            RightAlt = 1<<6,
            LeftCommand = 1<<7,
            RightCommand = 1<<8,
            NumLock = 1<<9,
            CapsLock = 1<<10,
            AltGr = 1<<11,
            Scroll = 1<<12,
            AnyControl = LeftControl | RightControl,
            AnyShift = LeftShift | RightShift,
            AnyAlt = LeftAlt | RightAlt,
            AnyCommand = LeftCommand | RightCommand
        };
    
        enum class KeyboardKey
        {
            Key0,
            Key1,
            Key2,
            Key3,
            Key4,
            Key5,
            Key6,
            Key7,
            Key8,
            Key9,
            KeyA,
            KeyB,
            KeyC,
            KeyD,
            KeyE,
            KeyF,
            KeyG,
            KeyH,
            KeyI,
            KeyJ,
            KeyK,
            KeyL,
            KeyM,
            KeyN,
            KeyO,
            KeyP,
            KeyQ,
            KeyR,
            KeyS,
            KeyT,
            KeyU,
            KeyV,
            KeyW,
            KeyX,
            KeyY,
            KeyZ
        };
    
        enum class KeyboardScancode
        {
            Key0,
            Key1,
            Key2,
            Key3,
            Key4,
            Key5,
            Key6,
            Key7,
            Key8,
            Key9,
            KeyA,
            KeyB,
            KeyC,
            KeyD,
            KeyE,
            KeyF,
            KeyG,
            KeyH,
            KeyI,
            KeyJ,
            KeyK,
            KeyL,
            KeyM,
            KeyN,
            KeyO,
            KeyP,
            KeyQ,
            KeyR,
            KeyS,
            KeyT,
            KeyU,
            KeyV,
            KeyW,
            KeyX,
            KeyY,
            KeyZ
        };
        
        struct KeyboardEvent
        {
        
        };
    }
    
    using namespace input;
    
    class Input
    {
        friend class MainLoop;
    private:
        virtual void update_inputs() = 0;
    public:
        virtual ~Input() = default;
        
        virtual bool get_key_down(KeyboardKey key, KeyboardModifiers modifiers) = 0;
        virtual bool get_key_up(KeyboardKey key, KeyboardModifiers modifiers) = 0;
        virtual bool get_key(KeyboardKey key, KeyboardModifiers modifiers) = 0;
        virtual bool get_physical_key_down(KeyboardScancode scancode, KeyboardModifiers modifiers) = 0;
        virtual bool get_physical_key_up(KeyboardScancode scancode, KeyboardModifiers modifiers) =  0;
        virtual bool get_physical_key(KeyboardScancode scancode, KeyboardModifiers modifiers) = 0;
        virtual String get_key_name(KeyboardKey key) = 0;
        virtual String get_scancode_name(KeyboardScancode scancode) = 0;
    
    
    private:

    };
}
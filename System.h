
#pragma once

#include "Archetype.h"

namespace vengine
{

    class System
    {
        friend class SystemManager;

    public:
        virtual ~System() = default;

    protected:
    public:
        virtual void on_update() {};
        virtual void on_register() { }
        virtual void on_deregister() { }

    private:
        String m_name;
    };
}
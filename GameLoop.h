#pragma once

#include "Context.h"
#include "SystemManager.h"
#include "Input.h"

namespace vengine
{
    class MainLoop
    {
    public:
        static void main_loop(Context& ctx)
        {
            while (true)
            {
                ctx.input().update_inputs();
                ctx.system_manager().run();
            }
        }
    };
}
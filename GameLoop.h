#include "Context.h"
#include "SystemManager.h"

namespace vengine
{
    void main_loop(Context& ctx)
    {
        while (1)
        {
            ctx.system_manager().run();
        }
    }
}
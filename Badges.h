#pragma once

namespace vengine
{
    class Context;
    namespace detail
    {
        class ContextBadge
        {
            friend Context;

        private:
            ContextBadge() = default;
        };
    }
}

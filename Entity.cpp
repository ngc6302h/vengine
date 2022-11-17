//
// Created by ngc on 4/11/22.
//
#include <Types.h>
#include "Entity.h"

namespace vengine
{
    u64 get_next_entity_id(u64 archetype_id)
    {
        static u64 next_id { 0 };
        return next_id++ | archetype_id << 44;
    }
}
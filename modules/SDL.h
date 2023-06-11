#pragma once
#include "../Input.h"
#include "../Window.h"
#include "../Subsystem.h"
#include "SDLStructs.h"

namespace vengine
{
    ResultOrError<SubsystemData, SubsystemCreationError> create_SDL2_subsystem(SDLWindowFlags flags);
}
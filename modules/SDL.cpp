#include "internal/SDL.h"
#include <Memory.h>
#include "SDL.h"
namespace vengine
{
    
    ResultOrError<SubsystemData, SubsystemCreationError> create_SDL2_subsystem(SDLWindowFlags flags)
    {
        // Initialize SDL2
        u32 sdl_flags = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS;
        if (flags & Vulkan) sdl_flags |= SDL_WINDOW_VULKAN;
        auto result = SDL_Init(sdl_flags);
        if (result != 0)
        {
            //TODO: Print error to stderr
            return SubsystemCreationError {"failed to initilize SDL (SDL_Init failed)"};
        }
        
        auto input_subsystem = create<SDLInput>();
        if (!input_subsystem.leak_ptr())
            return SubsystemCreationError("the SDL Input subsystem failed to be created");
        
        auto maybe_window_subsystem = SDLWindow::create(flags);
        if (maybe_window_subsystem.has_error())
            return SubsystemCreationError("the SDL Window subsystem failed to be created");
        
        SubsystemData data(maybe_window_subsystem.result(), input_subsystem.release_nonnull());
        return data;
    }
}
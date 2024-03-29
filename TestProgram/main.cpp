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

#include "../System.h"
#include "../EntityManager.h"
#include "../System.h"
#include "../Tasks.h"
#include "../SystemManager.h"
#include "../Context.h"
#include "../GameLoop.h"
#include "../Input.h"
#include "../vengine.h"
#include "../modules/SDL.h"

struct Position
{
    float x, y, z;
};

struct Velocity
{
    float vx, vy, vz;
};

struct Rotation
{
    float x, y, z, w;
};

struct Scale
{
    float x, y, z;
};

using namespace vengine;

class UpdatePhysicsSystem final : public System
{
public:
    ~UpdatePhysicsSystem() override = default;
    
    explicit UpdatePhysicsSystem(Context& ctx) : System("UpdatePhysicsSystem", ctx)
    {
    }
    
    struct TaskThatIteratesOverEveryMatchingEntity : public ParallelTask<TaskThatIteratesOverEveryMatchingEntity, Position, Velocity>
    {
        TaskThatIteratesOverEveryMatchingEntity()
        {
        
        }
        
        float delta_time;
        
        void execute(Position& position, Velocity& velocity)
        {
            position.x = position.x + velocity.vx * delta_time;
            position.y = position.y + velocity.vy * delta_time;
            position.z = position.z + velocity.vz * delta_time;
        }
    };
    
    struct TaskThatIteratesOverEveryMatchingEntityAndAlsoTakesAnIndex : public ParallelTaskWithIndex<TaskThatIteratesOverEveryMatchingEntityAndAlsoTakesAnIndex, Position>
    {
        void execute(u64 index, Position& position)
        {
        
        }
    };
    
    struct TaskThatIteratesOverAnUserProvidedBuffer : public CustomParallelTask<TaskThatIteratesOverAnUserProvidedBuffer>
    {
        Position* positions;
        Velocity* velocities;
        
        void execute(u64 index)
        {
        
        }
    };
    
    struct TaskThatExecutesOnce : public SingleTask<TaskThatExecutesOnce>
    {
        void execute()
        {
        
        }
    };
    
    void on_update() override
    {
        if (m_context.input().get_key_down(KeyboardKey::Key0, KeyboardModifiers::None))
            __builtin_printf("Key 0 pressed!\n");
        
        TaskThatIteratesOverEveryMatchingEntity* task1 = new TaskThatIteratesOverEveryMatchingEntity(); task1->delta_time = 0.02f;
        Task* task2 = new TaskThatIteratesOverEveryMatchingEntityAndAlsoTakesAnIndex();
        TaskThatIteratesOverAnUserProvidedBuffer* task3 = new TaskThatIteratesOverAnUserProvidedBuffer(); task3->positions = nullptr; task3->velocities = nullptr;
        Task* task4 = new TaskThatExecutesOnce();
        task1->depends_on(task2);
        task2->depends_on(task3, task4);
        
        task1->submit(context());
    }
};

int main()
{
#if DEBUG_ASSERTS == 1
    __builtin_printf("Debug asserts enabled\n");
#endif
    
    if (vengine::setup().is_error())
    {
        return -1;
    }
    
    auto maybe_sdl_subsystems = create_SDL2_subsystem(SDLWindowFlags::Vulkan);
    if (maybe_sdl_subsystems.has_error())
        return -1;
    Context ctx(std::move(maybe_sdl_subsystems.result()));
    __builtin_printf("Hello!\n");
    UpdatePhysicsSystem k(ctx);
    ctx.system_manager().register_system(&k);
    vengine::EntityID id = ctx.entity_manager().create(Position{}, Velocity{});
    vengine::EntityID id2 = ctx.entity_manager().create(Position{}, Rotation{}, Velocity{});
    vengine::EntityID id3 = ctx.entity_manager().create(Velocity{}, Position{}, Rotation{});
    vengine::EntityID id4 = ctx.entity_manager().create(Velocity{}, Position{}, Rotation{});
    id = ctx.entity_manager().add_component(id, Rotation{}).release_value();
    ctx.entity_manager().destroy(id);
    ctx.entity_manager().destroy(id4);
    ctx.entity_manager().destroy(id3);
    ctx.entity_manager().destroy(id2);
    vengine::EntityID id5 = ctx.entity_manager().create(Position{}, Rotation{}, Scale{});
    ctx.entity_manager().destroy(id5);
    ctx.archetype_manager().print_archetype_hierarchy();
    vengine::MainLoop::main_loop(ctx);
    return 0;
}

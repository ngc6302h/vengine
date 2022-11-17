

#include "../System.h"
#include "../EntityManager.h"
#include "../System.h"
#include "../Tasks.h"
#include "../SystemManager.h"
#include "../Context.h"

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

};

using namespace vengine;

class UpdatePhysicsSystem final : public System
{
public:
    ~UpdatePhysicsSystem() override = default;
    
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
    
    void update()
    {
        TaskThatIteratesOverEveryMatchingEntity* task1 = new TaskThatIteratesOverEveryMatchingEntity(); task1->delta_time = 0.02f;
        Task* task2 = new TaskThatIteratesOverEveryMatchingEntityAndAlsoTakesAnIndex();
        TaskThatIteratesOverAnUserProvidedBuffer* task3 = new TaskThatIteratesOverAnUserProvidedBuffer(); task3->positions = nullptr; task3->velocities = nullptr;
        Task* task4 = new TaskThatExecutesOnce();
        task1->depends_on(task2);
        task2->depends_on(task3, task4);
        
        task1->submit();
    }
};

int main()
{
    Context ctx;
    __builtin_printf("Hello!\n");
    UpdatePhysicsSystem k;
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
    return 0;
}

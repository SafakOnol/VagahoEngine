#include "ECS.h"


int Entity::GetId() const
{
    return id;
}

void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
    // Modern C++ flex
    entities.erase(std::remove_if(
        entities.begin(),
        entities.end(),
        [&entity](Entity other) { return entity.GetId() == other.GetId(); }), 
        entities.end());

    /*auto it = std::find_if(entities.begin(), entities.end(),
        [&entity](const Entity& e) { return e.GetId() == entity.GetId(); });
    if (it != entities.end()) {
        *it = std::move(entities.back());
        entities.pop_back();
    }*/


}

std::vector<Entity> System::GetSystemEntities() const
{
    return entities;
}

const Signature& System::GetComponentSignature() const
{
    return componentSignature;
}

#include "ECS.h"

int Entity::GetID() const
{
    return id;
}

int Component::GetID() const
{
    return id;
}

void System::AddEntityToSystem(Entity entity)
{
}

void System::RemoveEntityFromSystem(Entity entity)
{
}

std::vector<Entity> System::GetSystemEntities() const
{
    return std::vector<Entity>();
}

Signature& System::GetComponentSignature() const
{
    // TODO: insert return statement here
}

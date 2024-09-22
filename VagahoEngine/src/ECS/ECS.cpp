#include "ECS.h"

int Entity::GetId() const
{
    return id;
}

int Component::GetId() const
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

const Signature& System::GetComponentSignature() const
{
    // TODO: insert return statement here
}

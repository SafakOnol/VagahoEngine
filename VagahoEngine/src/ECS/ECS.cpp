#include "ECS.h"
#include "../Logger/Logger.h"

EntityId Entity::GetId() const {
    return id;
}

void System::AddEntityToSystem(Entity entity) {
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
    ////////////////////
    /// Modern C++ flex
    ////////////////////
    // remove_if() does not remove anything. 
    // It just swaps elements around inside the vector in order to put all elements 
    // that don't match the lambda criteria towards the front of the container.
    // After the remove_if swapped all the elements around keeping their order, 
    // it will then return an iterator to the first element that matched the lambda criteria.
    /*entities.erase(
        std::remove_if(
            entities.begin(),
            entities.end(),
            [&entity](Entity other) { return entity.GetId() == other.GetId(); }), 
            // [&entity](Entity other) { return entity == other; }), // this is with operator overloading
        entities.end());*/

    ///////////////////////////////
    /// Opeartor overloading method
    ///////////////////////////////
    // With addition of operator overloading in Entity class, we can use remove function instead of remove_if
    entities.erase(
        std::remove(
            entities.begin(),
            entities.end(),
            entity),
        entities.end()
    );

    /////////////////////////
    /// Classiscal C++ method
    /////////////////////////
    /*auto it = std::find_if(entities.begin(), entities.end(),
        [&entity](const Entity& e) { return e.GetId() == entity.GetId(); });
    if (it != entities.end()) {
        *it = std::move(entities.back());
        entities.pop_back();
    }*/

}

std::vector<Entity> System::GetSystemEntities() const {
    return entities;
}

const Signature& System::GetComponentSignature() const {
    return componentSignature;
}

Entity ECSManager::CreateEntity() {
    EntityId entityId;
    entityId = entityCount++;

    Entity entity(entityId);
    entitiesToCreate.insert(entity);
    return entity;

    LOG_INFO("Entity created with id = " + std::to_string(entityId));
}

void ECSManager::Update() {
    // TODO..
}

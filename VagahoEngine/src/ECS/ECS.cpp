#include "ECS.h"


ComponentId IComponent::nextId = 0;

EntityId Entity::GetId() const {
    return id;
}

void Entity::Destroy() {
    ecsManager->DestroyEntity(*this);
    LOG_INFO("Entity #" + std::to_string(this->GetId()) + " destroyed");
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

    if (freeIds.empty()) {
        // If there are no free ids waiting to be reused
        entityId = entityCount++;
        // Ensure the entityComponentSignatures vector has enough space
        if (entityId >= entityComponentSignatures.size()) {
            entityComponentSignatures.resize(entityId + 1);
        }
    }
    else {
        // Reuse an id from the list of previously removed entities.
        entityId = freeIds.front();
        freeIds.pop_front();
    }    

    Entity entity(entityId);
    entity.ecsManager = this;
    entitiesToCreate.insert(entity);  

    LOG_INFO("Entity created with id = " + std::to_string(entityId));

    return entity;    
}

void ECSManager::DestroyEntity(Entity entity) {
    entitiesToDestroy.insert(entity);
}

void ECSManager::AddEntityToSystems(Entity entity) {
    const auto entityId = entity.GetId();
    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    for (auto system : systems) {
        const auto& sytemComponentSignature = system.second->GetComponentSignature();

        // bitwise sorcery
        bool isInterested = (entityComponentSignature & sytemComponentSignature) == sytemComponentSignature;

        if (isInterested) {
            system.second->AddEntityToSystem(entity);
        }
    }

}

void ECSManager::RemoveEntityFromSystems(Entity entity) {
    for (auto system : systems) {
        system.second->RemoveEntityFromSystem(entity);
    }
}

void ECSManager::Update() {
    // Process the entities waiting to be created to the active Systems
    for (auto entity : entitiesToCreate) {
        AddEntityToSystems(entity);
    }
    entitiesToCreate.clear();

    // Process the entities waiting to be destroyed from the active Systems
    for (auto entity : entitiesToDestroy) {
        RemoveEntityFromSystems(entity);
        entityComponentSignatures[entity.GetId()].reset();

        // Make the entity Id available to be reused
        freeIds.push_back(entity.GetId());
    }
    entitiesToDestroy.clear();
}

#pragma once

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <cstdint>
#include <set>
#include <memory>
#include <algorithm>
#include <deque>

#include "../Logger/Logger.h"


using EntityId		= uint32_t;
using ComponentId	= uint32_t;
constexpr EntityId		INVALID_ENTITY_ID		= std::numeric_limits<EntityId>::max();
constexpr ComponentId	INVALID_COMPONENT_ID	= std::numeric_limits<ComponentId>::max();

const unsigned int MAX_COMPONENTS = 32;

////////////////////////////////////////////////
/// Signature
///////////////////////////////////////////////
/// Bitset (1s and 0s) are used to keep track of which components an entity has,
/// and to query which entitities a system is interested in
///////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

///////////////
/// ENTITY
///////////////

class Entity {
private:
	EntityId id;

public:
	Entity(EntityId id) : id(id) {} // Constructor using id
	Entity(const Entity& entity) = default;
	EntityId GetId() const;
	void Destroy();
	
	// overload & create default assignment operator
	Entity& operator =(const Entity& other) = default;

	// overload operators to compare ids.
	bool operator ==(const Entity& other) const { return id == other.id; }
	bool operator !=(const Entity& other) const { return id != other.id; }
	bool operator >(const Entity& other) const { return id > other.id; }
	bool operator <(const Entity& other) const { return id < other.id; }

	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> void RemoveComponent();
	template <typename TComponent> bool bHasComponent() const;
	template <typename TComponent> TComponent& GetComponent() const;

	// Hold a pointer to the entity's owner ecsManager (forward declaration!!!)
	class ECSManager* ecsManager;
};

///////////////
/// COMPONENT
///////////////

// Base component similar to an Interface, hens the I... naming convention similar to Unity
class IComponent {
protected:
	static ComponentId nextId;
};

// template component class, create an Id if being called for the first time, or return the id if it's already created
template <typename T>
class Component: public IComponent {
public:
	// return the unique id of Component<T>
	static ComponentId GetId() {
		static auto id = nextId++;
		return id;
	}
};

///////////////////////////////////////////////////////////////////
/// SYSTEM
///////////////////////////////////////////////////////////////////
/// The System processes entities that contain a specific signature
///////////////////////////////////////////////////////////////////
class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;
public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	// Define the component type T that entities must have
	template <typename TComponent> void AddRequiredComponent();
};

/////////////////////////////////////////////////////////////
/// Pool
/////////////////////////////////////////////////////////////
/// A pool is a vector (contiguous data) of objects of type T 
/////////////////////////////////////////////////////////////
class IPool {
public:
	virtual ~IPool(){

	}
};

template <typename T>
class Pool : public IPool {
private:
	std::vector<T> data;

public:
	Pool(int size = 100) {
		data.resize(size);
	}

	virtual ~Pool() = default;

	bool bIsComponentPoolEmpty() const{
		return data.empty();
	}

	// Get pool size (entities in a component pool)
	int GetComponentPoolSize() const {
		return data.size();
	}

	void ResizeComponentPool(int n) {
		data.resize(n);
	}

	/// TODO: find a better name for Clear and Add, make them more readable.
	void Clear() {
		data.clear();
	}

	// Add entity to a component pool vector (the list of entities that has this very component)
	void Add(T object) {
		data.push_back(object);
	}

	// Set the new entity in the pool vector to a specific index
	void SetComponentToEntityId(int index, T object) {
		data[index] = object;
	}

	T& GetComponentForEntityId(int index) {
		return static_cast<T&>(data[index]);
	}

	T& operator [](unsigned int index) {
		return data[index];
	}

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ECS MANAGER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ECS Manager is in charge of creation and destruction of entities, adding and removing systems and components
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ECSManager {
private:
	EntityId entityCount = 0;

	// set if entities to be added or removed in the next registry Update()
	std::set<Entity> entitiesToCreate;
	std::set<Entity> entitiesToDestroy;
	// Vector of component pools, each pool contains all the data for certain a component type
	// [Vector index = component type id]
	// [Pool index = entity id]
	// By using IPool, a parent class to Pool (similar to an interface) we are bypassing the requirement
	// of specifying the type of the pool (<T>).
	std::vector<std::shared_ptr<IPool>> componentPools;
	// Vector of component signatures per entity
	// specifies whith compoenents are turend on for that entity
	// [Vector index = entityid]
	std::vector<Signature> entityComponentSignatures;

	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

	// List of free entitiy ids that were previosly removed
	std::deque<int> freeIds;

public:
	// ECSManager() = default;
	ECSManager() { LOG_INFO("ECSManager constructor called!"); }
	~ECSManager() { LOG_INFO("ECSManager destructor called!"); }


	void Update();

	/// Entity Functions
	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	// Check the component signature of an entity and add the entity to the interested system
	void AddEntityToSystems(Entity entity);
	void RemoveEntityFromSystems(Entity entity);

	
	/// Component Functions
	// Add a component of type TComponent to an entity
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	// Remove a component of type TComponent from an entity
	template <typename TComponent> void RemoveComponent(Entity entity);
	// Check if a certain component is attached to a certain entity
	template <typename TComponent> bool bHasComponent(Entity entity) const;
	template <typename TComponent> TComponent& GetComponent(Entity entity) const;

	/// System Functions
	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool bHasSystem() const;
	template <typename TSystem> TSystem& GetSystem() const;
	
};

/// IMPLEMENTATION ///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////
/// COMPONENT TEMPLATES
///////////////////////

template<typename TComponent>
inline void System::AddRequiredComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

template<typename TComponent, typename ...TArgs>
inline void ECSManager::AddComponent(Entity entity, TArgs && ...args)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// check if componentId is greater than the current size of componentPools
	// resize the pools vector if required
	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	// if component type doesn't have its own pool, create one
	if (!componentPools[componentId]) {
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentId] = newComponentPool;
	}

	// Fetch the pool of component values for that component type
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

	// Check if the entity id is greater than the current size of the component pool
	// resize the pool if required
	if (entityId >= componentPool->GetComponentPoolSize()) {
		componentPool->ResizeComponentPool(entityId + 1);
	}

	// Create a new Component object of the type TComponent and forward the parameters to the constructor
	TComponent newComponent(std::forward<TArgs>(args)...);

	// Add the new component to the component pool list, using the entity id as index
	componentPool->SetComponentToEntityId(entityId, newComponent);
	
	// Change the component signature of the entity and set componenId on the bitset to 1
	entityComponentSignatures[entityId].set(componentId);

	// LOG_INFO("Component id = '" + std::to_string(componentId) + "' was added to Entity id = '" + std::to_string(entityId) + "'");
}

template<typename TComponent>
inline void ECSManager::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	entityComponentSignatures[entityId].set(componentId, false);
	// LOG_INFO("Component id: = '" + std::to_string(componentId) + "' was removed from the Entity id = '" + std::to_string(entityId) + "'");
}

template<typename TComponent>
inline bool ECSManager::bHasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	// test checks if componentId at the specific entityId is turned on in the bitset
	bool result = entityComponentSignatures[entityId].test(componentId);
	/*if (result) {
		LOG_INFO("Entity id = '" + std::to_string(entityId) + "' does have the Component id = '" + std::to_string(componentId) + "'");
	}
	else {
		LOG_WARNING("Entity id = '" + std::to_string(entityId) + "' doesn't have the Component id = '" + std::to_string(componentId) + "'");
	}*/
	
	return result;
	
}

template<typename TComponent>
inline TComponent& ECSManager::GetComponent(Entity entity) const
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	//LOG_INFO("Component id = '" + std::to_string(componentId) + "' was received from Entity id '" + std::to_string(entityId) + "'");
	return componentPool->GetComponentForEntityId(entityId);
}


////////////////////
/// SYSTEM TEMPLATES
////////////////////

template<typename TSystem, typename ...TArgs>
inline void ECSManager::AddSystem(TArgs && ...args) {
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	/// - my eyese are burning...
	// typeid is a modern C++ function that allows us to get the id of a system from Template
	// type index is the key to the newSystem value
	// Modern C++ syntax baby! 
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
inline void ECSManager::RemoveSystem() {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template<typename TSystem>
inline bool ECSManager::bHasSystem() const {
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();	
}

template<typename TSystem>
inline TSystem& ECSManager::GetSystem() const {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

////////////////////
/// ENTITY TEMPLATES
////////////////////

template<typename TComponent, typename ...TArgs>
inline void Entity::AddComponent(TArgs && ...args) {
	// ecsManager.AddComponent...
	ecsManager->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
inline void Entity::RemoveComponent() {
	ecsManager->RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
inline bool Entity::bHasComponent() const {
	return ecsManager->bHasComponent<TComponent>(*this);
}

template<typename TComponent>
inline TComponent& Entity::GetComponent() const {
	return ecsManager->GetComponent<TComponent>(*this);
}

#pragma once

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <cstdint>
#include <set>



using EntityId		= uint32_t;
using ComponentId	= uint32_t;
constexpr EntityId INVALID_ENTITY_ID		= std::numeric_limits<EntityId>::max();
constexpr ComponentId INVALID_COMPONENT_ID	= std::numeric_limits<ComponentId>::max();

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
	Entity(EntityId id) : id(id) {}; // Constructor using id
	Entity(const Entity& entity) = default;
	EntityId GetId() const;
	
	// overload & create default assignment operator
	Entity& operator =(const Entity& other) = default;

	// overload operators to compare ids.
	bool operator ==(const Entity& other) const { return id == other.id; }
	bool operator !=(const Entity& other) const { return id != other.id; }
	bool operator >(const Entity& other) const { return id > other.id; }
	bool operator <(const Entity& other) const { return id < other.id; }

};

///////////////
/// COMPONENT
///////////////

// Base component similar to an Interface, hens the I... naming convention similar to Unity
struct IComponent {
protected:
	static ComponentId nextId;
};

// assign a unique id to a component type
template <typename T>
class Component: public IComponent {
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
	template <typename TComponent> void ReqiureComponent();
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

	bool isEmpty() const{
		return data.empty();
	}

	int GetSize() const {
		return data.size();
	}

	void Resize(int n) {
		data.resize(n);
	}

	void Clear() {
		data.clear();
	}

	void Add(T object) {
		data.push_back(object);
	}

	void Set(int index, T object) {
		data[index] = object;
	}

	T& Get(int index) {
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
	uint32_t numEntities = 0;

	// set if entities to be added or removed in the next registry Update()
	std::set<Entity> entitiesToCreate;
	std::set<Entity> entitiesToDestroy;


	// Vector of component pools, each pool contains all the data for certain a component type
	// [Vector index = component type id]
	// [Pool index = entity id]
	// By using IPool, a parent class to Pool (similar to an interface) we are bypassing the requirement
	// of specifying the type of the pool (<T>).
	std::vector<IPool*> componentPools;

	// Vector of component signatures per entity
	// specifies whith compoenents are turend on for that entity
	// [Vector index = entityid]
	std::vector<Signature> entityComponentSignatures;

	std::unordered_map<std::type_index, System*> systems;

public:
	ECSManager() = default;
	void Update();

	Entity CreateEntity();
	void AddEntityToSystem(Entity entity);
	

	// TODO:
	
};

/// IMPLEMENTATION

template <typename TComponent>
void System::ReqiureComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

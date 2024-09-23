#pragma once

#include <bitset>
#include <vector>

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
	int id;

public:
	Entity(int id) : id(id) {}; // Constructor using id
	Entity(const Entity& entity) = default;
	int GetId() const;
	
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
	static int nextId;
};

// assign a unique id to a component type
template <typename T>
class Component: public IComponent {
	// return the unique id of Component<T>
	static int GetId() {
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

class EntityManager {
	// TODO...
};

/// IMPLEMENTATION

template <typename TComponent>
void System::ReqiureComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

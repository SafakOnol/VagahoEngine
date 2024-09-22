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

class Entity {
private:
	int id;

public:
	Entity(int id) : id(id) {};
	int GetId() const;
	// ...
};

// Base component similar to an Interface, hens the I... naming convention similar to Unity
struct IComponent {
protected:
	static int nexId;
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
/// The System processes entities that contain a specific signature
///////////////////////////////////////////////////////////////////
class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;
public:
	System() = default;
	virtual ~System() = default;

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

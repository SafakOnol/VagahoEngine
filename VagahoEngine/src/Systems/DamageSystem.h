#pragma once

#include "../ECS/ECS.h"

#include "../Components/BoxColliderComponent.h"
#include "../EventManager/EventManager.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System {
public:
	DamageSystem() {
		AddRequiredComponent<BoxColliderComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager) {
		eventManager->Subscribe<CollisionEvent>(this, &DamageSystem::OnCollision);
	}

	void OnCollision(CollisionEvent& event) {
		LOG_INFO("The Damage system received and event collision between entities " + std::to_string(event.a.GetId()) + " and " + std::to_string(event.b.GetId()));
		// event.a.Destroy();
		// event.b.Destroy();
	}

	void Update() {

	}
};

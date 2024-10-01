#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"

class MovementSystem : public System {
public:
	MovementSystem() {
		AddRequiredComponent<TransformComponent>();
		AddRequiredComponent<RigidbodyComponent>();
	};

	void Update(double deltaTime) {
		
		for (auto entity : GetSystemEntities()) {
			TransformComponent& transform = entity.GetComponent<TransformComponent>(); // call this as reference since we are changing the current value
			const RigidbodyComponent rigidbody = entity.GetComponent<RigidbodyComponent>();

			transform.position.x += rigidbody.velocity.x * deltaTime;
			transform.position.y += rigidbody.velocity.y * deltaTime;
			
			/*LOG_INFO(
				"Entity id = " 
				+ std::to_string(entity.GetId()) 
				+ " position: (" + std::to_string(transform.position.x) 
				+ ", " + std::to_string(transform.position.y) + ")"
			);*/
		}
	
		
	};
};
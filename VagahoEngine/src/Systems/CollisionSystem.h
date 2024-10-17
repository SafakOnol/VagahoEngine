#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

class CollisionSystem : public System {
public:
	CollisionSystem() {
		AddRequiredComponent<TransformComponent>();
		AddRequiredComponent<BoxColliderComponent>();
	}

	void Update() {
		
		struct CollisionEntity {
			TransformComponent transformComponent;
			BoxColliderComponent boxColliderComponent;
		};

		std::vector<CollisionEntity> collisionEntities;
		for (auto entity : GetSystemEntities()) {
			CollisionEntity collisionEntity;
			collisionEntity.transformComponent = entity.GetComponent<TransformComponent>();
			collisionEntity.boxColliderComponent = entity.GetComponent<BoxColliderComponent>();
			collisionEntities.emplace_back(collisionEntity);
			
		}
		// LOG_INFO(std::to_string(collisionEntities.size()));

		//LOG_INFO(std::to_string(collisionEntities.size()));
		//if (1 < collisionEntities.size()) {
		//	// LOG_INFO(std::to_string(collisionEntities.at(0).transformComponent.position.x));
		//}
		//else {
		//	LOG_ERROR("Index out of bounds");
		//	return;
		//}
		//

		
		


		if (collisionEntities.size() > 0)
		{
			auto pos1x = collisionEntities.at(0).transformComponent.position.x;
			auto pos1y = collisionEntities.at(0).transformComponent.position.y;
			auto pos2x = collisionEntities.at(1).transformComponent.position.x;
			auto pos2y = collisionEntities.at(1).transformComponent.position.y;
			auto box1h = collisionEntities.at(0).boxColliderComponent.height;
			auto box1w = collisionEntities.at(0).boxColliderComponent.width;
			auto box2h = collisionEntities.at(1).boxColliderComponent.height;
			auto box2w = collisionEntities.at(1).boxColliderComponent.width;

			if (
				pos1x < (pos2x + box2w) &&
				(pos1x + box1w) > pos2x &&
				pos1y < (pos2y + box2h) &&
				(pos1y + box1h) > pos2y				
				)
			{
				LOG_WARNING("COLLISION DETECTED!!!");
			}

		}

	}
};

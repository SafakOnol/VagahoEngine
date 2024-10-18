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

		auto collisionEntities = GetSystemEntities();

		for (std::vector<Entity>::iterator i = collisionEntities.begin(); i != collisionEntities.end(); i++)
		{
			Entity entityFirst = *i;
			auto entityFirstTransform		= entityFirst.GetComponent<TransformComponent>();
			auto entityFirstBoxCollider		= entityFirst.GetComponent<BoxColliderComponent>();



			for (std::vector<Entity>::iterator j = i; j != collisionEntities.end(); j++)
			{
				Entity entitySecond = *j;

				// bypass if comparing same entities
				if (entityFirst == entitySecond) continue;

				auto entitySecondTransform		= entitySecond.GetComponent<TransformComponent>();
				auto entitySecondBoxCollider	= entitySecond.GetComponent<BoxColliderComponent>();
				
				// check collision between a and b
				bool bCollisionOnGoing = CheckAABBCollision(
					entityFirstTransform.position.x + entityFirstBoxCollider.offset.x,
					entityFirstTransform.position.y + entityFirstBoxCollider.offset.y,
					entityFirstBoxCollider.width,
					entityFirstBoxCollider.height,
					entitySecondTransform.position.x + entitySecondBoxCollider.offset.x,
					entitySecondTransform.position.y + entitySecondBoxCollider.offset.y,
					entitySecondBoxCollider.width,
					entitySecondBoxCollider.height
				);

				if (bCollisionOnGoing) {
					LOG_WARNING("Entity " + std::to_string(entityFirst.GetId()) + " is colliding with " + std::to_string(entitySecond.GetId()));
				}
			}
		}
		
	}

	bool CheckAABBCollision(
		double aX, double aY, double aW, double aH,
		double bX, double bY, double bW, double bH
	) {
		return (
			aX < bX + bW &&
			aX + aW > bX &&
			aY < bY + bH &&
			aY + aH > bY
			);
	}
};

#pragma once

#include "../ECS/ECS.h"
#include "../Utils/HashUtils.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

#include <unordered_map>
#include <unordered_set>

class CollisionSystem : public System {

private:
	// AABB (Axis-Aligned Bounding Box) collision check
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

	// Map to keep track of ongoing collisions
	// Use PairHash as the hash function for our pair of entity IDs
	std::unordered_map<std::pair<int, int>, bool, PairHash> collisionMap;

	// Helper function to create a unique pair of entity IDs
	// Creates a unique, consistently ordered pair of entity IDs
	// Ensures CreateEntityPair(A, B) == CreateEntityPair(B, A) via min, max
	// Used for consistent collision tracking and efficient lookup
	std::pair<int, int> CreateEntityPair(int id1, int id2) {
		return std::make_pair(std::min(id1, id2), std::max(id1, id2));
	}

	// Called when a collision starts between two entities
	void OnCollisionStart(Entity& entity1, Entity& entity2) {
		LOG_INFO("Collision started between Entity " + std::to_string(entity1.GetId()) + " and Entity " + std::to_string(entity2.GetId()));
		// TODO: Add broadcast function here.
	}

	void OnCollisionStay(Entity& entity1, Entity& entity2) {
		// LOG_INFO("Collision ongoing between Entity " + std::to_string(entity1.GetId()) + " and Entity " + std::to_string(entity2.GetId()));
		// TODO: Replace with broadcast or event system call
		// Add logic for continuous collision handling
	}

	// Called when a collision ends between two entities
	void OnCollisionEnd(int entityId1, int entityId2) {
		LOG_INFO("Collision ended between Entity " + std::to_string(entityId1) + " and Entity " + std::to_string(entityId2));
		// TODO: Add broadcast function here.
	}


public:
	// Constructor: Adds required components for collision detection
	CollisionSystem() {
		AddRequiredComponent<TransformComponent>();
		AddRequiredComponent<BoxColliderComponent>();
	}

	// Main update function called every frame
	void Update() {
		// Get all entities with required components for collision
		auto collisionEntities = GetSystemEntities();
		// Set to keep track of collisions in the current frame
		std::unordered_set<std::pair<int, int>, PairHash> currentCollisions;

		// Outer loop: iterate through all entities
		for (std::vector<Entity>::iterator i = collisionEntities.begin(); i != collisionEntities.end(); i++)
		{
			Entity entityFirst = *i;
			// Get components of the first entity
			auto entityFirstTransform		= entityFirst.GetComponent<TransformComponent>();
			auto entityFirstBoxCollider		= entityFirst.GetComponent<BoxColliderComponent>();

			// Inner loop: compare with all other entities
			for (std::vector<Entity>::iterator j = i; j != collisionEntities.end(); j++)
			{
				Entity entitySecond = *j;
				// bypass if comparing same entities
				if (entityFirst == entitySecond) continue;
				// Get components of the second entity
				auto entitySecondTransform		= entitySecond.GetComponent<TransformComponent>();
				auto entitySecondBoxCollider	= entitySecond.GetComponent<BoxColliderComponent>();
				
				// Check for collision between the two entities
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

				// Create a unique pair for these two entities
				// This ensures that the pair (A,B) is the same as (B,A) for consistent tracking
				auto entityPair = CreateEntityPair(entityFirst.GetId(), entitySecond.GetId());

				if (bCollisionOnGoing) {
					// LOG_WARNING("Entity " + std::to_string(entityFirst.GetId()) + " is colliding with " + std::to_string(entitySecond.GetId()));
					// A collision is currently happening between entityFirst and entitySecond
					
					// Add this collision pair to the set of current collisions
					currentCollisions.insert(entityPair);
					// Check if this collision is new or was inactive in the previous frame
					// collisionMap.find(entityPair) == collisionMap.end(): Checks if this pair isn't in the map (new collision)
					// !collisionMap[entityPair]: Checks if this pair is in the map but was inactive (false)
					if (collisionMap.find(entityPair) == collisionMap.end() || !collisionMap[entityPair]) {
						// This is either a new collision or a collision that was previously inactive
						// Mark this collision as active in the collisionMap
						collisionMap[entityPair] = true;

						// Trigger collision start event
						// Pass the full Entity objects to allow access to all entity data if needed
						OnCollisionStart(entityFirst, entitySecond);
					}
					else {
						// Ongoing collision
						OnCollisionStay(entityFirst, entitySecond);
					}
					
				}
			}
		}

		// Check for collisions that have ended
		// Iterate through the collision map to check for collisions that have ended
		for (auto it = collisionMap.begin(); it != collisionMap.end(); ) {
			// Check if this collision was active in the previous frame (it->second is true)
			// AND if it's not present in the current frame's collisions
			if (it->second && currentCollisions.find(it->first) == currentCollisions.end()) {
				// This collision was happening in the previous frame but not in the current frame,
				// which means it has just ended

				// Extract the entity IDs from the map key (it->first is a pair of entity IDs)
				int entityId1 = it->first.first;
				int entityId2 = it->first.second;

				// Call the collision end event handler
				OnCollisionEnd(entityId1, entityId2);
				// Remove this collision from the map since it's no longer active
				// erase() returns the next valid iterator, so we don't need to increment it
				it = collisionMap.erase(it);
			}
			else {
				// If the collision is still ongoing or wasn't active in the previous frame,
				// move to the next item in the map
				++it;
			}
		}	
	}
};

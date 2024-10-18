#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Systems/CollisionSystem.h"

#include <SDL2/SDL.h>

class CollisionRenderSystem : public System {
public:
	CollisionRenderSystem() {
		AddRequiredComponent<TransformComponent>();
		AddRequiredComponent<BoxColliderComponent>();
	}

	void Update(SDL_Renderer* renderer) {
		for (auto entity : GetSystemEntities()) {
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto collider = entity.GetComponent<BoxColliderComponent>();

			SDL_Rect colliderRect = {
				static_cast<int>(transform.position.x + collider.offset.x),
				static_cast<int>(transform.position.y + collider.offset.y),
				static_cast<int>(collider.width * transform.scale.x),
				static_cast<int>(collider.height * transform.scale.y)
			};
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &colliderRect); // draws an unfilled rectangle
		}
	}
};
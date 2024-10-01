#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"

#include <SDL2/SDL.h>

class RenderSystem : public System {
public:
	RenderSystem() {
		AddRequiredComponent<TransformComponent>();
		AddRequiredComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer) {
		for (auto entity : GetSystemEntities()) {
			const TransformComponent transform = entity.GetComponent<TransformComponent>();
			const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();

			SDL_Rect entityRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				sprite.width,
				sprite.height
			};
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &entityRect);
		}
	}
};


#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetManager/AssetManager.h"

#include <SDL2/SDL.h>

class RenderSystem : public System {
public:
	RenderSystem() {
		AddRequiredComponent<TransformComponent>();
		AddRequiredComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager) {
		for (auto entity : GetSystemEntities()) {
			const TransformComponent transform = entity.GetComponent<TransformComponent>();
			const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();
			
			// Set the source rectangle of original sprite texture
			SDL_Rect srcRect = sprite.srcRect;
			// Set the destination rectangle woth the x,y position
			SDL_Rect destRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			// Draw PNG texture
			SDL_RenderCopyEx(
				renderer,
				assetManager->GetTexture(sprite.assetId),
				&srcRect,
				&destRect,
				transform.rotation,
				NULL, // Center point
				SDL_FLIP_NONE
			);
		}
	}
};


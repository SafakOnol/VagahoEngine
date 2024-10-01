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
			SDL_Texture* texture = assetManager->GetTexture(sprite.assetId);

			// Draw Rectangle
			/*SDL_Rect entityRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				sprite.width,
				sprite.height
			};
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &entityRect);*/


			// Set the source rectangle of original sprite texture
			SDL_Rect srcRect = sprite.srcRect;
			if (srcRect.h == 0 && srcRect.h == 0)
				SDL_QueryTexture(texture, NULL, NULL, &srcRect.w, &srcRect.h);
			auto spriteWidth = sprite.width;
			auto spriteHeight = sprite.height;
			if (spriteWidth == 0 && spriteHeight == 0) {
				spriteWidth = srcRect.w;
				spriteHeight = srcRect.h;
			}


			// Set the destination rectangle woth the x,y position
			SDL_Rect destRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(spriteWidth * transform.scale.x),
				static_cast<int>(spriteHeight * transform.scale.y)
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


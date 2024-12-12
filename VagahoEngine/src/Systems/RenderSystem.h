#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetManager/AssetManager.h"

#include <SDL.h>
#include <algorithm>

class RenderSystem : public System {
public:
	RenderSystem() {
		AddRequiredComponent<TransformComponent>();
		AddRequiredComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera) {
		// Create a vector with both Sprite and Transform component of all entities

		struct RenderableEntity {
			TransformComponent transformComponent;
			SpriteComponent spriteComponent;
		};

		std::vector<RenderableEntity> renderableEntities;
		for (auto entity : GetSystemEntities()) {
			RenderableEntity renderableEntity;
			renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
			renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
			renderableEntities.emplace_back(renderableEntity);
		}

		// Sort the vector by z-Index
		std::sort(
			renderableEntities.begin(), 
			renderableEntities.end(), 
			[](const RenderableEntity& a, const RenderableEntity& b) {
				return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
			});
		

		for (auto entity : renderableEntities) {
			const TransformComponent transform = entity.transformComponent;
			const SpriteComponent sprite = entity.spriteComponent;
			
			// Set the source rectangle of original sprite texture
			SDL_Rect srcRect = sprite.srcRect;
			// Set the destination rectangle woth the x,y position
			SDL_Rect destRect = {
				static_cast<int>(transform.position.x - (sprite.bIsFixed ? 0 : camera.x)),
				static_cast<int>(transform.position.y - (sprite.bIsFixed ? 0 : camera.y)),
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


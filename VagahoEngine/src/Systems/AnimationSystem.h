#pragma once

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"

class AnimationSystem : public System {
public:
	AnimationSystem() {
		AddRequiredComponent<SpriteComponent>();
		AddRequiredComponent<AnimationComponent>();
	}

	void Update() {
		for (auto entity : GetSystemEntities()) {
			AnimationComponent& animation	= entity.GetComponent<AnimationComponent>();
			SpriteComponent&	sprite		= entity.GetComponent<SpriteComponent>();

			animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameChangeRate / 1000) % animation.totalFrames;
			sprite.srcRect.x = animation.currentFrame * sprite.width;
		}
	}
};
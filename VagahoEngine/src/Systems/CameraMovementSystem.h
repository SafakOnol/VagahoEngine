#pragma once

#include "../ECS/ECS.h"
#include "../Components/FollowCameraComponent.h"
#include "../Components/TransformComponent.h"

#include <SDL2/SDL.h>

class CameraMovementSystem : public System {
public:
	CameraMovementSystem() {
		AddRequiredComponent<FollowCameraComponent>();
		AddRequiredComponent<TransformComponent>();
	}

	void Update(SDL_Rect& camera) {
		for (Entity entity : GetSystemEntities()) {
			TransformComponent transform = entity.GetComponent<TransformComponent>();

			if (transform.position.x + (camera.w * 0.5) < Game::mapWidth) {
				camera.x = transform.position.x - (Game::windowWidth * 0.5);
			}
			if (transform.position.y + (camera.h * 0.5) < Game::mapHeight) {
				camera.y = transform.position.y - (Game::windowHeight * 0.5);
			}
			
			// Kepp camera rectangle view insite the screen limits
			camera.x = camera.x < 0 ? 0 : camera.x;
			camera.y = camera.y < 0 ? 0 : camera.y;
			camera.x = camera.x > camera.w ? camera.w : camera.x;
			camera.y = camera.y > camera.h ? camera.h : camera.y;
		}
	}
};
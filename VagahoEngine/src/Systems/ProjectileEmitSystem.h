#pragma once

#include "../ECS/ECS.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"

#include <SDL2/SDL.h>

class ProjectileEmitSystem : public System {
public:
	ProjectileEmitSystem() {
		AddRequiredComponent<ProjectileEmitterComponent>();
		AddRequiredComponent<TransformComponent>();
	}

	void Update(std::unique_ptr<ECSManager>& ecsManager) {
		for (Entity entity : GetSystemEntities()) {
			auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
			const auto emitterTransform = entity.GetComponent<TransformComponent>();

			if (SDL_GetTicks() - projectileEmitter.lastEmissionTimestamp > projectileEmitter.fireRate) {
				glm::vec2 projectilePosition = emitterTransform.position;
				if (entity.bHasComponent<SpriteComponent>()) {
					const auto sprite = entity.GetComponent<SpriteComponent>();
					projectilePosition.x += (emitterTransform.scale.x * sprite.width * 0.5);
					projectilePosition.y += (emitterTransform.scale.y * sprite.height * 0.5);
				}

				// spawn new projectile entity
				Entity projectile = ecsManager->CreateEntity();
				projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
				projectile.AddComponent<RigidbodyComponent>(projectileEmitter.projectileVelocity);
				projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 3);
				projectile.AddComponent<BoxColliderComponent>(4, 4);
				projectile.AddComponent<ProjectileComponent>(projectileEmitter.bIsFriendlyFire, projectileEmitter.damagePerHit, projectileEmitter.projectileLifetime);

				// Update timestamp
				projectileEmitter.lastEmissionTimestamp = SDL_GetTicks();
			}
		}
	}
};
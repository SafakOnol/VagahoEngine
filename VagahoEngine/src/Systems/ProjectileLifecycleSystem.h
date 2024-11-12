#pragma once

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"

class ProjectileLifecycleSystem : public System {
public:
	ProjectileLifecycleSystem() {
		AddRequiredComponent<ProjectileComponent>();
	}

	void Update() {
		for (auto entity : GetSystemEntities()) {
			auto projectile = entity.GetComponent<ProjectileComponent>();

			if (SDL_GetTicks() - projectile.spawnTimestamp > projectile.projectileLifetime) {
				entity.Destroy();
			}
		}
	}
};
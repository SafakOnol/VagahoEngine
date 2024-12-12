#pragma once

#include <SDL.h>
#include <glm/glm.hpp>

struct ProjectileEmitterComponent {
	glm::vec2	projectileVelocity;
	int			fireRate;
	int			projectileLifetime;
	int			damagePerHit;
	bool		bIsFriendlyFire;
	int			lastEmissionTimestamp;

	ProjectileEmitterComponent(	glm::vec2 projectileVelocity = glm::vec2(0), int fireRate = 0,
								int projectileLifetime = 10000, int damagePerHit = 10, bool bIsFriendlyFire = false) {
		this->projectileVelocity = projectileVelocity;
		this->fireRate = fireRate;
		this->projectileLifetime = projectileLifetime;
		this->damagePerHit = damagePerHit;
		this->bIsFriendlyFire = bIsFriendlyFire;
		this->lastEmissionTimestamp = SDL_GetTicks();
	}
};

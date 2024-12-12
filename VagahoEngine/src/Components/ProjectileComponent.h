#pragma once

#include <SDL.h>

struct ProjectileComponent {
	bool bIsFriendlyFire;
	int	damagePerHit;
	int	projectileLifetime;
	int spawnTimestamp;

	ProjectileComponent(bool bIsFriendlyFire = false, int damagePerHit = 0, int projectileLifetime = 0, int spawnTimestamp = 0) {
		this->bIsFriendlyFire = bIsFriendlyFire;
		this->damagePerHit = damagePerHit;
		this->projectileLifetime = projectileLifetime;
		this->spawnTimestamp = SDL_GetTicks();
	}
};
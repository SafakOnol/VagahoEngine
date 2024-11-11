#pragma once

struct HealthComponent {
	int currentHealth;
	int maxHealth;

	HealthComponent(int currentHealth = 100, int maxHealth = 100) {
		this->currentHealth = currentHealth;
		this->maxHealth = maxHealth;
	}
};
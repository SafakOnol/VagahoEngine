#pragma once

#include <SDL.h>

struct AnimationComponent {
	int totalFrames;
	int currentFrame;
	int frameChangeRate;
	bool bIsLooping;
	int startTime;

	AnimationComponent(int totalFrames = 1, int frameChangeRate = 1, bool bIsLooping = true) {
		this->totalFrames = totalFrames;
		this->currentFrame = 1;
		this->frameChangeRate = frameChangeRate;
		this->bIsLooping = bIsLooping;
		this->startTime = SDL_GetTicks();
	}

};

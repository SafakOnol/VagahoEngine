#pragma once

#include <string>
#include <SDL.h>

struct SpriteComponent {
	std::string assetId;
	int width;
	int height;
	int zIndex;
	bool bIsFixed;
	SDL_Rect srcRect;

	SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int zIndex = 0, bool bIsFixed = false,  int srcRectX = 0, int srcRectY = 0) {
		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->zIndex = zIndex;
		this->bIsFixed = bIsFixed;
		this->srcRect = { srcRectX, srcRectY, width, height };
	}
};
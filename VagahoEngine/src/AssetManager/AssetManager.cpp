#include "AssetManager.h"

#include "../Logger/Logger.h"

#include <SDL_image.h>

AssetManager::AssetManager() {
	// LOG_INFO("Asset Manager constructor called");
}

AssetManager::~AssetManager() {
	ClearAssets();
	// LOG_INFO("Asset Manager destructor called");
}

void AssetManager::ClearAssets() {
	for (auto texture : textures) {
		SDL_DestroyTexture(texture.second); // second is the value of key-value pair, the actual texture. fist is id
	}
	textures.clear();
}

void AssetManager::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filepath) {
	SDL_Surface* surface = IMG_Load(filepath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	// Add the texture to the map
	textures.emplace(assetId, texture);
	// LOG_INFO("Texture added. Texture Id: " + assetId);
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetId) {
	return textures[assetId];
}

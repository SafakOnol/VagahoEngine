#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>



class AssetManager {
private:
	std::map<std::string, SDL_Texture*> textures;



public:
	AssetManager();
	~AssetManager();

	void ClearAssets();
	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filepath);
	SDL_Texture* GetTexture(const std::string& assetId);

};


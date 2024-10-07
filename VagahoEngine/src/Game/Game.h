#pragma once

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"

const int FPS = 60;
const int FRAME_TIME_DURATION = 1000 / FPS;

class Game {
private:
	bool			bGameIsRunning;
	int				ticksPrevFrame;
	double			deltaTime;
	SDL_Window*		window;
	SDL_Renderer*	renderer;

	std::unique_ptr<ECSManager> ecsManager;
	std::unique_ptr<AssetManager> assetManager;

	std::vector<std::vector<int>> mapData;
	int mapWidth;
	int mapHeight;
	

public:
	Game();
	~Game();
	void Initialize();
	void Setup();
	void LoadLevel(int level);
	void LoadMap(const std::string& filename);
	void HandleFrameTime();
	void HandleInput();
	void Update();
	void Render();
	void Run();
	void Destroy();

	int windowWidth;
	int windowHeight;
};


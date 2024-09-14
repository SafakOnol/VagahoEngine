#pragma once

#include <SDL2/SDL.h>

class Game{
private:
	bool bGameIsRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	Game();
	~Game();
	void Initialize();
	void HandleInput();
	void Update();
	void Render();
	void Run();
	void Destroy();
};


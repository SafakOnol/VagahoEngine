#pragma once

#include <SDL2/SDL.h>

const int FPS = 60;
const int FRAME_TIME_DURATION = 1000 / FPS;

class Game {
private:
	bool bGameIsRunning;
	int ticksPrevFrame;
	float deltaTime;
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	Game();
	~Game();
	void Initialize();
	void Setup();
	void HandleFrameTime();
	void HandleInput();
	void Update();
	void Render();
	void Run();
	void Destroy();

	int windowWidth;
	int windowHeight;
};


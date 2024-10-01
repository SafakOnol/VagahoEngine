#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"

#include <SDL_image.h>
#include <glm/glm.hpp>

#include <iostream>


Game::Game() {
	bGameIsRunning = false;
	ticksPrevFrame = 0;
	deltaTime = 0;

	ecsManager = std::make_unique<ECSManager>();

	std::cout << "INITIAL TERMINAL COLOR" << std::endl;
	LOG_INFO("Game constructor called!");
}

Game::~Game() {
	LOG_INFO("Game destructor called!");
}

void Game::Initialize() {
	// Initialize SDL, window and renderer in this order
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		LOG_ERROR("Error initializing SDL.");
		return;
	}

	// Create SDL display mode struct and populate with get current display mode function
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	//windowWidth	= 800;
	//windowHeight	= 600;

	windowWidth		= 3440;
	windowHeight	= 1440;

	window = SDL_CreateWindow(
		"Vagaho Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		LOG_ERROR("Error creating SDL window.");
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		LOG_ERROR("Error creating SDL renderer.");
		return;
	}
	// Real Fullscreen mode (change video mode from os to app
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	bGameIsRunning = true;
}



void Game::Setup() {
	// TODO:
	// Entity enemy = registry.CreateEntity();
	// tank.AddComponent<TransformCOmponent>();
	// ...
	Entity entity01 = ecsManager->CreateEntity();

	/*ecsManager->AddComponent<TransformComponent>(entity01, glm::vec2(20.0, 20.0), glm::vec2(1.0, 1.0), 0.0);
	ecsManager->AddComponent<RigidbodyComponent>(entity01, glm::vec2(10.0, 0.0));*/

	entity01.AddComponent<TransformComponent>(glm::vec2(20.0, 20.0), glm::vec2(1.0, 1.0), 0.0);
	entity01.AddComponent<RigidbodyComponent>(glm::vec2(10.0, 0.0));
	entity01.bHasComponent<TransformComponent>();
	entity01.RemoveComponent<TransformComponent>();
	entity01.GetComponent<RigidbodyComponent>();
	entity01.bHasComponent<TransformComponent>();
	
}

void Game::HandleFrameTime() {

	// Lock Frame Rate
	int timeToDelay = FRAME_TIME_DURATION - (SDL_GetTicks() - ticksPrevFrame);

	if (timeToDelay > 0 && timeToDelay <= FRAME_TIME_DURATION) {
		SDL_Delay(timeToDelay);
	}

	// Calculate deltaTime in seconds
	deltaTime = (SDL_GetTicks() - ticksPrevFrame) / 1000.0f;

	ticksPrevFrame = SDL_GetTicks();
}

void Game::HandleInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
			case SDL_QUIT:
				bGameIsRunning = false;
				break;
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
					bGameIsRunning = false;
				}
				break;
		}
	}
}

void Game::Update() {

	HandleFrameTime();

	// TODO:
	// MovementSystem.Update();
	// CollisionSystem.Update();
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// TODO: Render game objects...

	//// Draw a PNG texture
	//SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	//SDL_FreeSurface(surface); // once the texture is created, surface is not needed anymore

	//// rectangle struct to draw the texture
	//SDL_Rect destinationRect = { 
	//	static_cast<int>(playerPosition.x), 
	//	static_cast<int>(playerPosition.y),
	//	32, 
	//	32 
	//};
	//SDL_RenderCopy(renderer, texture, NULL, &destinationRect); // NULL means we're copying the entire texture, not a subset of the texture rect (ie sprite anims)
	//SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}

void Game::Run() {
	Setup();
	while (bGameIsRunning) {
		HandleInput();
		Update();
		Render();
	}
}

void Game::Destroy() {
	// Destroy in reverse order
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

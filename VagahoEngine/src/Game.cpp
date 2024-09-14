#include "Game.h"

#include <SDL_image.h>
#include <glm/glm.hpp>

#include <iostream>


Game::Game() {
	bGameIsRunning = false;
	std::cout << "Game constructor called!" << std::endl;
}

Game::~Game() {
	std::cout << "Game destructor called!" << std::endl;
}

void Game::Initialize() {
	// Initialize SDL, window and renderer in this order
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL." << std::endl;
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
		std::cerr << "Error creating SDL window." << std::endl;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}
	// Real Fullscreen mode (change video mode from os to app
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	bGameIsRunning = true;
}

// temp
glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup() {
	playerPosition = glm::vec2(10.0f, 10.f);
	playerVelocity = glm::vec2(1.0f, 0.0f);
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
	playerPosition.x += playerVelocity.x;
	playerPosition.y += playerVelocity.y;
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// Draw a PNG texture
	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface); // once the texture is created, surface is not needed anymore

	// rectangle struct to draw the texture
	SDL_Rect destinationRect = { 
		static_cast<int>(playerPosition.x), 
		static_cast<int>(playerPosition.y),
		32, 
		32 
	};
	SDL_RenderCopy(renderer, texture, NULL, &destinationRect); // NULL means we're copying the entire texture, not a subset of the texture rect (ie sprite anims)
	SDL_DestroyTexture(texture);

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

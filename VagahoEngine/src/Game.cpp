#include "Game.h"

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
	window = SDL_CreateWindow(
		"Vagaho Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_ALWAYS_ON_TOP
	);
	if (!window) {
		std::cerr << "Error creating SDL window." << std::endl;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}
	bGameIsRunning = true;
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
}

void Game::Render() {
}

void Game::Run() {
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

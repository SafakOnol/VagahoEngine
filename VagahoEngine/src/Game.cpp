#include "Game.h"
#include <SDL2/SDL.h>
#include <iostream>

Game::Game() {
	std::cout << "Game constructor called!" << std::endl;
}

Game::~Game() {
	std::cout << "Game destructor called!" << std::endl;
}

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL." << std::endl;
		return;
	}
	SDL_Window* window = SDL_CreateWindow(
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
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
	}
}

void Game::HandleInput() {
}

void Game::Update() {
}

void Game::Render() {
}

void Game::Run() {
}

void Game::Destroy() {
}

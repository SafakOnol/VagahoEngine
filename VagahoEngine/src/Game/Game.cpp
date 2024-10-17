#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"

#include <SDL_image.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>



Game::Game() {
	bGameIsRunning	= false;
	ticksPrevFrame	= 0;
	deltaTime		= 0;

	ecsManager		= std::make_unique<ECSManager>();
	assetManager	= std::make_unique<AssetManager>();

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
	windowWidth		= 1600;
	windowHeight	= 1200;
	//windowWidth = displayMode.w;
	//windowHeight = displayMode.h;
	//windowWidth		= 3440;
	//windowHeight	= 1440;

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
	// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	bGameIsRunning = true;
}


void Game::LoadMap(const std::string& filename) {
	
}

void Game::LoadLevel(int level) {
	// Add systems that need to be processed in the game
	ecsManager->AddSystem<MovementSystem>();
	ecsManager->AddSystem<RenderSystem>();
	ecsManager->AddSystem<AnimationSystem>();
	ecsManager->AddSystem<CollisionSystem>();


	// Add assets to asset manager
	assetManager->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetManager->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
	assetManager->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
	assetManager->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
	assetManager->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-left.png");

	// TODO: Load tilemap
	const int TILESET_COLUMNS	= 10;	// Change this to match tileset grid
	const int TILESIZE			= 32;	// Change this to match tileset resolution
	double tileScale			= 2.0;	

	std::vector<std::vector<int>> mapData;
	std::ifstream mapFile("./assets/tilemaps/jungle.map");
	std::string line;
	
	while (std::getline(mapFile, line)) {
		std::vector<int> row;
		std::istringstream iss(line);
		int tileId;
		while (iss >> tileId) {
			row.push_back(tileId);
			if (iss.peek() == ',')
				iss.ignore();
		}
		mapData.push_back(row);
	}
	mapFile.close();

	for (int y = 0; y < mapData.size(); y++) {
		for (int x = 0; x < mapData[y].size(); x++) {
			int tileId = mapData[y][x];
			int srcRectY = (tileId / TILESET_COLUMNS) * TILESIZE;
			int srcRectX = (tileId % TILESET_COLUMNS) * TILESIZE;

			Entity tile = ecsManager->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * TILESIZE), y * (tileScale * TILESIZE)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", TILESIZE, TILESIZE, 0, srcRectX, srcRectY);
		}
	}

	// Load Entities and Components
	Entity tank01 = ecsManager->CreateEntity();
	tank01.AddComponent<TransformComponent>(glm::vec2(20.0, 20.0), glm::vec2(2.0, 2.0), 0.0);
	tank01.AddComponent<RigidbodyComponent>(glm::vec2(20.0, 0.0));
	tank01.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
	tank01.AddComponent<BoxColliderComponent>(32, 32);


	Entity truck01 = ecsManager->CreateEntity();
	truck01.AddComponent<TransformComponent>(glm::vec2(300.0, 20.0), glm::vec2(2.0, 2.0), 0.0);
	truck01.AddComponent<RigidbodyComponent>(glm::vec2(-20.0, 0.0));
	truck01.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
	truck01.AddComponent<BoxColliderComponent>(32, 32);
	

	Entity chopper = ecsManager->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(520.0, 200.0), glm::vec2(2.0, 2.0), 0.0);
	chopper.AddComponent<RigidbodyComponent>(glm::vec2(0.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 2);
	chopper.AddComponent<AnimationComponent>(2, 15, true);

	Entity radarScreen = ecsManager->CreateEntity();
	radarScreen.AddComponent<TransformComponent>(glm::vec2(windowWidth - (3*64), windowHeight - (3*64)), glm::vec2(2.0, 2.0), 0.0);
	radarScreen.AddComponent<RigidbodyComponent>(glm::vec2(0.0, 0.0));
	radarScreen.AddComponent<SpriteComponent>("radar-image", 64, 64, 3);
	radarScreen.AddComponent<AnimationComponent>(8, 8, true);
}

void Game::Setup() {
	LoadLevel(1);
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

	// Update all systems
	ecsManager->GetSystem<MovementSystem>().Update(deltaTime);
	ecsManager->GetSystem<AnimationSystem>().Update();
	ecsManager->GetSystem<CollisionSystem>().Update();




	//////////////////////////////////////////////////////
	/// END OF UPDATE
	/// !!! Update ECS Manager at the end of game update.
	ecsManager->Update();
	//////////////////////////////////////////////////////
}



void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// Update all systems that requires rendering
	ecsManager->GetSystem<RenderSystem>().Update(renderer, assetManager);

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

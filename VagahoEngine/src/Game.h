#pragma once


class Game{
private:
	// ..

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


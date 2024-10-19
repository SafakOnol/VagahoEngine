#pragma once

#include "../ECS/ECS.h"

#include "../EventManager/EventManager.h"
#include "../Events/KeyPressedEvent.h"

class KeyboardControlSystem : public System {
public:
	KeyboardControlSystem() {

	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager) {
		eventManager->Subscribe<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& event) {
		std::string keyCode = std::to_string(event.symbol);
		std::string keySymbol(1, event.symbol);
		LOG_WARNING("Key pressed event broadcasted: [" + keyCode + "] " + keySymbol);
	}
	
	void Update() {

	}
};

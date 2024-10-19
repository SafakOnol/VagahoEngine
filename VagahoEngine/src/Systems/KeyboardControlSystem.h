#pragma once

#include "../ECS/ECS.h"

#include "../EventManager/EventManager.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"

class KeyboardControlSystem : public System {
public:
	KeyboardControlSystem() {
		AddRequiredComponent<KeyboardControlComponent>();
		AddRequiredComponent<RigidbodyComponent>();
		AddRequiredComponent<SpriteComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager) {
		eventManager->Subscribe<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& event) {
		/*std::string keyCode = std::to_string(event.symbol);
		std::string keySymbol(1, event.symbol);
		LOG_WARNING("Key pressed event broadcasted: [" + keyCode + "] " + keySymbol);*/

		for (auto entity : GetSystemEntities()) {
			const auto keyboardControl = entity.GetComponent<KeyboardControlComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& rigidbody = entity.GetComponent<RigidbodyComponent>();

			switch (event.symbol) {
			case SDLK_UP: 
				rigidbody.velocity = keyboardControl.upVelocity;
				sprite.srcRect.y = sprite.height * 0;
				break;
			case SDLK_RIGHT:
				rigidbody.velocity = keyboardControl.rightVelocity;
				sprite.srcRect.y = sprite.height * 1;
				break;
			case SDLK_DOWN:
				rigidbody.velocity = keyboardControl.downVelocity;
				sprite.srcRect.y = sprite.height * 2;
				break;
			case SDLK_LEFT:
				rigidbody.velocity = keyboardControl.leftVelocity;
				sprite.srcRect.y = sprite.height * 3;
				break;
			}
		}

	}
	
	void Update() {

	}
};

#pragma once

#include "../ECS/ECS.h"
#include "../EventManager/Event.h"

class CollisionEvent : public Event {
public:
	Entity a;
	Entity b;
	CollisionEvent(Entity a, Entity b) : a(a), b(b) {}
};
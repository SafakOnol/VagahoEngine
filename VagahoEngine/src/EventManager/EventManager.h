#pragma once

#include "../Logger/Logger.h"
#include "Event.h"

#include <map>
#include <typeindex>
#include <memory>
#include <list>

// Abstract base class defines the interface for all event listeners
class IEventListener {
private:
	// Pure virtual function to handle events, to be implemented by derived classes
	virtual void HandleEvent(Event& event) = 0;

public:
	// Virtual destructor for proper cleanup of derived classes
	virtual ~IEventListener() = default;

	// Public method to notify the listener of an event
	// This method calls the private HandleEvent method
	void NotifyListener(Event& event) {
		HandleEvent(event);
	}
};

// Concrete implementation of IEventListener
// This class template allows for type-safe event handling
template <typename TOwner, typename TEvent>
class EventListener : public IEventListener {
private:
	// Type alias for the event handler function pointer
	typedef void (TOwner::* EventHandlerFunction)(TEvent&);

	TOwner* ownerPtr;					// Pointer to the owner object
	EventHandlerFunction handlerFunc;	// Function pointer to the event handler

	// Implementation of the HandleEvent method
	// This method invokes the handler function on the owner object
	virtual void HandleEvent(Event& event) override {
		std::invoke(handlerFunc, ownerPtr, static_cast<TEvent&>(event));
	}

public:
	EventListener(TOwner* ownerPtr, EventHandlerFunction handlerFunc)
		: ownerPtr(ownerPtr), handlerFunc(handlerFunc) {}

	virtual ~EventListener() override = default;

	// Method to check if this listener belongs to a specific owner
	bool OwnerMatches(TOwner* otherOwnerPtr) const {
		return ownerPtr == otherOwnerPtr;
	}

	// Method to check if this listener uses a specific handler function
	bool HandlerMatches(EventHandlerFunction otherHandlerFunc) const {
		return handlerFunc == otherHandlerFunc;
	}
};

// Type alias for a list of unique pointers to IEventListener
typedef std::list<std::unique_ptr<IEventListener>>ListenerList;

class EventManager {
private:
	// Map to store listeners for each event type
	// The key is the type_index of the event, and the value is a unique_ptr to a ListenerList
	std::map<std::type_index, std::unique_ptr<ListenerList>> listeners;
public:
	EventManager() {
		LOG_INFO("EventManager constructor called!");
	}
	~EventManager() {
		LOG_INFO("EventManager destructor called!");
	}

	//////////////////////////////////
	/// Subscribe to an event type <T>
	//////////////////////////////////
	// Template function to subscribe an owner to an event of type TEvent
	// TEvent: The type of event to subscribe to
	// TOwner: The type of the object owning the handler function
	template <typename TEvent, typename TOwner>
	void Subscribe(TOwner* ownerPtr, void (TOwner::* handlerFunc)(TEvent&)) {
		// Check if a listener list for this event type already exists
		// typeid(TEvent) provides a unique identifier for the event type
		// .get() is used to access the raw pointer from the unique_ptr
		if (!listeners[typeid(TEvent)].get()) {
			// If no listener list exists, create a new one
			// std::make_unique creates a new ListenerList wrapped in a unique_ptr
			listeners[typeid(TEvent)] = std::make_unique<ListenerList>();
		}
		// Create a new EventListener object
		// This object links the owner (ownerPtr) with its event handling function (handlerFunc)
		// std::make_unique is used to create a unique_ptr managing this new EventListener
		auto newListener = std::make_unique<EventListener<TOwner, TEvent>>(ownerPtr, handlerFunc);
		// Add the new listener to the list of listeners for this event type
		// std::move is used to transfer ownership of the unique_ptr to the list
		listeners[typeid(TEvent)]->push_back(std::move(newListener));
	}

	// clear subscriber list
	void Reset() {
		listeners.clear();
	}

	//////////////////////////////////////
	/// Unsubscribe from an event type <T>
	//////////////////////////////////////
	template <typename TEvent, typename TOwner>
	void Unsubscribe(TOwner* ownerPtr, void (TOwner::* handlerFunc)(TEvent&)) {
		// Get the list of listeners for this event type
		auto currentListeners = listeners[typeid(TEvent)].get();
		if (currentListeners) {
			// Use remove_if to eliminate matching listeners
			currentListeners->remove_if([ownerPtr, handlerFunc](const auto& listener) { // use Lambda function to acces ownerPtr and handlerFunc 
				// Attempt to cast the listener to the specific EventListener type
				// This cast is necessary because we store listeners as IEventListener pointers
				auto* listenerToRemove = dynamic_cast<EventListener<TOwner, TEvent>*>(listener.get());
				// The listener should be removed if:
				return	listenerToRemove &&									// 1. The cast was successful (listener is of the correct type)
						listenerToRemove->OwnerMatches(ownerPtr) &&			// 2. The owner matches
						listenerToRemove->HandlerMatches(handlerFunc);		// 3. The handler function matches
				// If all conditions are true, this listener will be removed from the list
				});
			// After this operation, all matching listeners will be removed from the list
		}
		// If currentListeners is null, it means there are no listeners for this event type, so we don't need to do anything
	}


	//////////////////////////////////
	/// Broadcast an event type <T>
	//////////////////////////////////

	// Template function to broadcast an event of type TEvent
	// TArgs... allows for variadic arguments to construct the event
	template <typename TEvent, typename ...TArgs>
	void BroadcastEvent(TArgs && ...args) {
		// Get the list of listeners for this specific event type
		// typeid(TEvent) is used as the key to find the correct listener list
		auto currentListeners = listeners[typeid(TEvent)].get();
		// Check if there are any listeners registered for this event type
		if (currentListeners) {
			// Iterate through all listeners for this event type
			for (auto it = currentListeners->begin(); it != currentListeners->end(); it++) {
				// Get a raw pointer to the current listener
				// We use get() because the listeners are stored as unique_ptr
				auto listener = it->get();
				// Construct the event object
				// std::forward is used for perfect forwarding of arguments
				// This allows for both lvalue and rvalue arguments to be correctly forwarded
				TEvent event(std::forward<TArgs>(args)...);

				// Notify the listener by calling its NotifyListener method
				// This will internally call the HandleEvent method of the specific listener
				listener->NotifyListener(event);
			}
		}
	}

};

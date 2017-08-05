#pragma once

#include <string>

class Entity;

class Component
{
public:

	enum MessageType {
		MSG_UPDATE,
		MSG_RENDER
	};
	
	virtual ~Component() {};

	virtual void receive(Entity& e, MessageType msg) = 0;

	virtual std::string getType() = 0;

	bool enabled_ = true;

protected:
	// To stop instantiation leading to calling getType() = 0
	Component() {};
};


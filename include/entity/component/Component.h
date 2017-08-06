#pragma once

#include <string>

class Entity;

enum class MessageType {
	MSG_UPDATE,
	MSG_RENDER
};

class Component
{
public:

	virtual ~Component() {};

	virtual void receive(Entity& e, MessageType msg) = 0;

	virtual std::string getType() = 0;

	bool enabled_ = true;

protected:
	// To stop instantiation leading to calling getType() = 0
	Component() {};
};


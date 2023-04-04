#pragma once

#include <string>

class Entity;

enum class MessageType
{
	MSG_UPDATE,
	MSG_RENDER
};

class Component
{
public:
	Component(){};

	inline void setDeleting(bool deleting)
	{
		deleting_ = deleting;
	}

	virtual void receive(Entity &e, MessageType msg) = 0;

	virtual std::string getType() = 0;

	virtual ~Component(){};

protected:
	bool enabled_ = true;
	bool deleting_ = false;
};

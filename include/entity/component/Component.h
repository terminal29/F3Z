#pragma once

#include <string>

class Entity;

enum class MessageType
{
	MSG_UPDATE,
	MSG_RENDER
};

class ComponentBase
{
public:
	inline void setDeleting(bool deleting)
	{
		deleting_ = deleting;
	}

	virtual void receive(Entity &e, MessageType msg) = 0;

	virtual ~ComponentBase() {}

	using typeid_t = void (*)();

protected:
	template <typename T>
	static typeid_t type_id() noexcept
	{
		return typeid_t(type_id<T>);
	}

	bool enabled_ = true;
	bool deleting_ = false;
};

template <typename Derived>
class Component : public ComponentBase
{
public:
	static typeid_t getTypeID() noexcept
	{
		return type_id<Derived>();
	};

protected:
	virtual ~Component(){};
};

#pragma once

#include <string>
#include <memory>

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
		if (deleting)
		{
			setEntity(nullptr);
		}
	}

	inline bool isDeleting() const
	{
		return deleting_;
	}

	virtual void receive(MessageType msg) = 0;

	inline void setEntity(Entity *entity)
	{
		entity_ = entity;
	}

	Entity *getEntity() const
	{
		return entity_;
	}

	virtual void onCreate(){};

	virtual ~ComponentBase() {}

	using typeid_t = void (*)();

protected:
	template <typename T>
	static typeid_t type_id() noexcept
	{
		return typeid_t(type_id<T>);
	}

	bool enabled_{true};
	bool deleting_{false};
	Entity *entity_{nullptr};
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

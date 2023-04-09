#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Entity;

enum class MessageType {
    MSG_UPDATE = 0,
    MSG_RENDER_FOREGROUND,
    MSG_RENDER_MID,
    MSG_RENDER_BACKGROUND
};

extern std::unordered_map<MessageType, std::string> MessageTypeNames;

class ComponentBase {
public:
    inline void setDeleting(bool deleting) noexcept
    {
        deleting_ = deleting;
        if (deleting) {
            setEntity(nullptr);
        }
    }

    inline bool isDeleting() const noexcept
    {
        return deleting_;
    }

    virtual void receive(MessageType msg) = 0;

    inline void setEntity(Entity* entity) noexcept
    {
        entity_ = entity;
    }

    Entity* getEntity() const noexcept
    {
        return entity_;
    }

    virtual void onCreate() {};

    virtual ~ComponentBase() { }

    using typeid_t = void (*)();

    virtual typeid_t getInstanceTypeID() const noexcept = 0;

    virtual std::shared_ptr<ComponentBase> clone() const noexcept = 0;

protected:
    template <typename T>
    static typeid_t type_id() noexcept
    {
        return typeid_t(type_id<T>);
    }

    bool enabled_ { true };
    bool deleting_ { false };
    Entity* entity_ { nullptr };
};

template <typename Derived>
class Component : public ComponentBase {
public:
    static typeid_t getTypeID() noexcept
    {
        return type_id<Derived>();
    };

    typeid_t getInstanceTypeID() const noexcept override
    {
        return getTypeID();
    }

    inline virtual std::shared_ptr<ComponentBase> clone() const noexcept
    {
        return std::make_shared<Derived>(static_cast<Derived const&>(*this));
    }

protected:
    virtual ~Component() {};
};

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace baba {
class Entito;
}

enum class MessageType {
    MSG_UPDATE = 0,
    MSG_RENDER_FOREGROUND,
    MSG_RENDER_MID,
    MSG_RENDER_BACKGROUND,
    MSG_PHYSICS_TICK
};

extern std::unordered_map<MessageType, std::string> MessageTypeNames;

class ComponentBase {
public:
    ComponentBase() = default;

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

    inline void setEntity(baba::Entito* entity) noexcept
    {
        if (entity_) {
            onDetach();
        }
        entity_ = entity;
        if (entity) {
            onAttach(entity);
        }
    }

    baba::Entito* getEntity() const noexcept
    {
        return entity_;
    }

    // called after setEntity
    virtual void onCreate() {};

    virtual void onAttach(baba::Entito* entity) {};

    virtual void onDetach() {};

    virtual ~ComponentBase() { }

    using typeid_t = void (*)();

    virtual typeid_t getInstanceTypeID() const noexcept = 0;

    virtual std::shared_ptr<ComponentBase> clone() const noexcept = 0;

    // copy constructor
    // loses entity reference
    ComponentBase(const ComponentBase& other)
    {
        entity_ = nullptr;
        deleting_ = false;
    }

    // copy assignment
    // loses entity reference
    ComponentBase& operator=(const ComponentBase& other)
    {
        entity_ = nullptr;
        deleting_ = false;
        return *this;
    }

protected:
    template <typename T>
    static typeid_t type_id() noexcept
    {
        return typeid_t(type_id<T>);
    }

    bool enabled_ { true };
    bool deleting_ { false };
    baba::Entito* entity_ { nullptr };
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

    inline virtual std::shared_ptr<ComponentBase> clone() const noexcept final
    {
        return std::make_shared<Derived>(static_cast<Derived const&>(*this));
    }

protected:
    virtual ~Component() {};
};

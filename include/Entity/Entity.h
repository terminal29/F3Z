#pragma once
#include <memory>
#include <vector>

#include <c3d++/C3DModel.h>
#include <c3d++/C3DTransform.h>
#include <entity/component/Component.h>

#include <iostream>

class WorldComponent;

template <typename T>
concept is_component_subtype = requires {
    requires std::derived_from<T, Component<T>>;
};

// template <typename T>
// concept is_component_base = requires {
//     std::is_same<T*, ComponentBase*>::value;
// };

class Entity {
public:
    Entity();
    ~Entity();

    Entity(C3DModel model);
    Entity(C3DModel model, C3DTransform transform);

    Entity(const Entity& other)
        : name_(other.name_)
        , model_(other.model_)
        , world_(other.world_)
        , transform_(other.transform_)
    {
        for (auto& component : other.components_) {
            cloneComponent(*component.second);
        }
        for (const auto& otherChild : other.children_) {
            children_[otherChild.first] = std::make_shared<Entity>(*otherChild.second);
        }
    }

    Entity& operator=(Entity const& other)
    {
        Entity tmp(other);
        tmp.swap(*this);
        return *this;
    };

    /* World getters and setters */
    void setWorld(std::weak_ptr<WorldComponent> world);
    std::weak_ptr<WorldComponent> getWorld() const;

    void setName(std::string name);
    std::string const& getName() const;

    /* Transform getters & setters */
    C3DTransform const& getTransform();
    void setTransform(C3DTransform transform);

    /* Model getters & setters */
    C3DModel const& getModel();
    void setModel(C3DModel model);

    void receive(MessageType type);

    std::string addChild(Entity child);
    bool removeChildByName(std::string name);
    std::weak_ptr<Entity> findChildByName(std::string name);

    friend std::ostream& operator<<(std::ostream& stream, const Entity& entity)
    {
        stream << entity.getName() << std::endl;
        for (const auto& child : entity.children_) {
            stream << "> " << *child.second << std::endl;
        }
        return stream;
    }

    /* Add a component to this entity, fails if there is already a component of
     * that type attached to the entity */
    template <typename TComponentType>
        requires is_component_subtype<TComponentType>
    bool addComponent(TComponentType component)
    {
        auto id = TComponentType::getTypeID();
        if (hasComponent<TComponentType>()) {
            return false;
        }
        components_[id] = std::make_shared<TComponentType>(std::move(component));
        components_[id]->setEntity(this);
        components_[id]->onCreate();
        return true;
    }

    /* Get a component from this entity, returns nullptr if there is no component
     * of that type attached to the entity */
    template <typename TComponentType>
        requires is_component_subtype<TComponentType>
    std::weak_ptr<TComponentType> getComponent()
    {
        auto id = TComponentType::getTypeID();
        if (hasComponent<TComponentType>()) {
            return std::static_pointer_cast<TComponentType>(components_.at(id));
        }
        return std::weak_ptr<TComponentType>();
    }

    /* Get a component from this entity, returns nullptr if there is no component
     * of that type attached to the entity */
    template <typename TComponentType>
        requires is_component_subtype<TComponentType>
    std::weak_ptr<TComponentType const> getComponent() const
    {
        auto id = TComponentType::getTypeID();
        if (hasComponent<TComponentType>()) {
            return std::static_pointer_cast<TComponentType const>(components_[id]);
        }
        return std::weak_ptr<TComponentType const>();
    }

    /* Get a component from this entity, returns nullptr if there is no component
     * of that type attached to the entity */
    std::weak_ptr<ComponentBase> getComponentById(ComponentBase::typeid_t id)
    {
        if (hasComponentById(id)) {
            return components_.at(id);
        }
        return std::weak_ptr<ComponentBase>();
    }

    /* Get a component from this entity, returns nullptr if there is no component
     * of that type attached to the entity */
    std::weak_ptr<ComponentBase const> getComponentById(ComponentBase::typeid_t id) const
    {
        if (hasComponentById(id)) {
            return std::const_pointer_cast<const ComponentBase>(components_.at(id));
        }
        return std::weak_ptr<ComponentBase const>();
    }

    /* Check if this entity has a component of the given type */
    template <typename TComponentType>
        requires is_component_subtype<TComponentType>
    bool hasComponent() const
    {
        auto id = TComponentType::getTypeID();
        return hasComponent(id);
    }

    bool hasComponent(ComponentBase::typeid_t id) const
    {
        return components_.contains(id);
    }

    template <typename TComponentType>
        requires is_component_subtype<TComponentType>
    bool removeComponent()
    {
        if (components_.contains(TComponentType::getTypeID())) {
            auto& component = components_.at(TComponentType::getTypeID());
            component->setDeleting(true);
            components_.erase(TComponentType::getTypeID());
            return true;
        }
        return false;
    }

    /* Check if this entity has a component of a type */
    template <typename TComponentType>
        requires is_component_subtype<TComponentType>
    bool hasComponent()
    {
        return components_.contains(TComponentType::getTypeID());
    }

    bool hasComponentById(ComponentBase::typeid_t id) const
    {
        return components_.contains(id);
    }

private:
    std::string name_;
    C3DModel model_;
    C3DTransform transform_;
    std::weak_ptr<WorldComponent> world_;
    std::unordered_map<ComponentBase::typeid_t, std::shared_ptr<ComponentBase>>
        components_;
    std::unordered_map<std::string, std::shared_ptr<Entity>> children_;

    void swap(Entity& other)
    {
        std::swap(name_, other.name_);
        std::swap(model_, other.model_);
        std::swap(transform_, other.transform_);
        std::swap(world_, other.world_);
        std::swap(components_, other.components_);
        std::swap(children_, other.children_);
    }

    inline bool cloneComponent(ComponentBase const& component)
    {
        auto id = component.getInstanceTypeID();
        if (components_.contains(id)) {
            return false;
        }
        auto clone = component.clone();
        clone->setEntity(this);
        clone->onCreate();
        components_[id] = std::move(clone);
        return true;
    }
};
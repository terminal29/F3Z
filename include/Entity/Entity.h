#pragma once
#include <vector>
#include <memory>

#include <entity/component/Component.h>
#include <c3d++/C3DModel.h>
#include <c3d++/C3DTransform.h>

class WorldComponent;

class Entity
{
public:
	Entity();
	~Entity();

	Entity(C3DModel model);
	Entity(C3DModel model, C3DTransform transform);

	/* World getters and setters */
	void setWorld(std::weak_ptr<WorldComponent> world);
	std::weak_ptr<WorldComponent> getWorld() const;

	/* Transform getters & setters */
	C3DTransform const &getTransform();
	void setTransform(C3DTransform transform);

	/* Model getters & setters */
	C3DModel const &getModel();
	void setModel(C3DModel model);

	void receive(MessageType type);

	/* Add a component to this entity, fails if there is already a component of that type attached to the entity */
	template <typename TComponentType>
		requires std::derived_from<TComponentType, Component<TComponentType>> bool
	addComponent(TComponentType component)
	{
		auto id = TComponentType::getTypeID();
		if (hasComponent<TComponentType>())
		{
			return false;
		}
		components_[id] = std::make_shared<TComponentType>(std::move(component));
		components_[id]->setEntity(this);
		components_[id]->onCreate();
		return true;
	}

	/* Get a pointer to a component, will be nullptr if the component doesnt exist */
	template <typename TComponentType>
		requires std::derived_from<TComponentType, Component<TComponentType>>
	std::weak_ptr<TComponentType> getComponent()
	{
		if (components_.contains(TComponentType::getTypeID()))
		{
			// Safe to cast as we know the type
			return std::weak_ptr<TComponentType>(std::static_pointer_cast<TComponentType>(components_.at(TComponentType::getTypeID())));
		}
		else
		{
			return std::weak_ptr<TComponentType>();
		}
	}

	template <typename TComponentType>
		requires std::derived_from<TComponentType, Component<TComponentType>> bool
	removeComponent()
	{
		if (components_.contains(TComponentType::getTypeID()))
		{
			auto &component = components_.at(TComponentType::getTypeID());
			component->setDeleting(true);
			components_.erase(TComponentType::getTypeID());
			return true;
		}
		return false;
	}

	/* Check if this entity has a component of a type */
	template <typename TComponentType>
		requires std::derived_from<TComponentType, Component<TComponentType>> bool
	hasComponent()
	{
		return components_.contains(TComponentType::getTypeID());
	}

private:
	C3DModel model_;
	C3DTransform transform_;
	std::weak_ptr<WorldComponent> world_;
	std::unordered_map<ComponentBase::typeid_t, std::shared_ptr<ComponentBase>> components_;
};

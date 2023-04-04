#pragma once
#include <vector>
#include <memory>

#include <entity/component/Component.h>
#include <c3d++/C3DModel.h>
#include <c3d++/C3DTransform.h>

class Entity
{
public:
	Entity();
	~Entity();

	Entity(C3DModel model);
	Entity(C3DModel model, C3DTransform transform);

	/* Transform getters & setters */
	C3DTransform const &getTransform();
	void setTransform(C3DTransform transform);

	/* Model getters & setters */
	C3DModel const &getModel();
	void setModel(C3DModel model);

	void receive(MessageType type);

	/* Add a component to this entity, fails if there is already a component of that type attached to the entity */
	template <typename TComponentType>
		requires std::derived_from<TComponentType, Component> bool
	addComponent(TComponentType component)
	{
		auto type = component.getType();
		if (hasComponent(type))
		{
			return false;
		}
		components_[type] = std::make_shared<TComponentType>(component);
		return true;
	}

	/* Get a pointer to a component, will be nullptr if the component doesnt exist */
	std::weak_ptr<Component> getComponent(std::string componentType);

	/* Delete a component (not implemented yet) */
	bool removeComponent(std::string componentType);

	/* Check if this entity has a component of a type */
	bool hasComponent(std::string componentType);

private:
	C3DModel model_;
	C3DTransform transform_;
	std::unordered_map<std::string, std::shared_ptr<Component>> components_;
};

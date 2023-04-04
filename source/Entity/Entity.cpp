#include <entity/Entity.h>

Entity::Entity()
{
}

Entity::Entity(C3DModel model, C3DTransform transform) : model_(model), transform_(transform)
{
}

Entity::Entity(C3DModel model) : model_(model)
{
}

Entity::~Entity()
{
}

C3DTransform const &Entity::getTransform()
{
	return transform_;
}
void Entity::setTransform(C3DTransform transform)
{
	transform_ = transform;
}

C3DModel const &Entity::getModel()
{
	return model_;
}

void Entity::setModel(C3DModel model)
{
	model_ = model;
}

void Entity::receive(MessageType message)
{
	for (auto &component : components_)
	{
		component.second->receive(*this, message);
	}
}

bool Entity::hasComponent(std::string componentType)
{
	return components_.contains(componentType);
}

std::weak_ptr<Component> Entity::getComponent(std::string componentType)
{
	if (hasComponent(componentType))
	{
		return std::weak_ptr(components_.at(componentType));
	}
	return std::weak_ptr<Component>();
}

bool Entity::removeComponent(std::string componentType)
{
	if (hasComponent(componentType))
	{
		components_.at(componentType)->setDeleting(true);
		components_.erase(componentType);
		return true;
	}
	return false;
}

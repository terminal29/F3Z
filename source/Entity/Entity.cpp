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
	for (auto &component : components_)
	{
		component.second->setDeleting(true);
	}
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
		component.second->receive(message);
	}
	for (auto &component : components_)
	{
		if (component.second->isDeleting())
		{
			components_.erase(component.first);
		}
	}
}

void Entity::setWorld(std::weak_ptr<WorldComponent> world)
{
	world_ = world;
}

std::weak_ptr<WorldComponent> Entity::getWorld() const
{
	return world_;
}

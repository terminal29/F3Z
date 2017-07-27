#include "entity/Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

Transform Entity::getTransform() {
	return transform_;
}
void Entity::setTransform(Transform transform) {
	transform_ = transform;
}

Model Entity::getModel() {
	return model_;
}

void Entity::setModel(Model model) {
	model_ = model;
}

void Entity::addComponent(Component* component) {
	components_.push_back(component);
}

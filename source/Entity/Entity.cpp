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
	model_.setDirty();
}

bool Entity::addComponent(Component* component) {
	if (hasComponent(component->getType())) {
		return false;
	}
	components_.push_back(component);
	return true;
}

bool Entity::hasComponent(std::string componentType) {
	for (int i = 0; i < components_.size(); i++) {
		if (components_.at(i)->getType() == componentType) {
			return true;
		}
	}
	return false;
}

Component* Entity::getComponent(std::string componentType) {
	for (int i = 0; i < components_.size(); i++) {
		if (components_.at(i)->getType() == componentType) {
			return components_.at(i);
		}
	}
	return nullptr;
}


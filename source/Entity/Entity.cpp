#include <entity/Entity.h>

Entity::Entity()
{
}

Entity::Entity(C3DModel model, C3DTransform transform) :model_(model), transform_(transform)
{
}

Entity::Entity(C3DModel model) : model_(model)
{
}

Entity::~Entity()
{
}

C3DTransform Entity::getTransform() {
	return transform_;
}
void Entity::setTransform(C3DTransform transform) {
	transform_ = transform;
}

C3DModel Entity::getModel() {
	return model_;
}

void Entity::setModel(C3DModel model) {
	model_ = model;
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

bool Entity::removeComponent(std::string componentType) {
	// TODO
	return false;
}

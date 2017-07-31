#pragma once
#include "component\Component.h"
#include "component/Model.h"
#include "component/Transform.h"

#include <vector>

class Entity
{
public:
	Entity();
	~Entity();


	Entity(Model model, Transform t);
	Entity(Model model);

	/* Transform getters & setters */
	Transform getTransform();
	void setTransform(Transform transform);

	/* Model getters & setters */
	Model getModel();
	void setModel(Model model);

	/* Add a component to this entity, fails if there is already a component of that type attached to the entity */
	bool addComponent(Component* component);

	/* Get the pointer to a component */
	Component* getComponent(std::string componentType);

	/* Check if this entity has a component of a type */
	bool hasComponent(std::string componentType);

private:
	Model model_;
	Transform transform_;
	std::vector<Component*> components_;

};


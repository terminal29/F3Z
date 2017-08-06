#pragma once
#include <vector>

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
	C3DTransform& getTransform();
	void setTransform(C3DTransform transform);

	/* Model getters & setters */
	C3DModel& getModel();
	void setModel(C3DModel model);

	void receive(MessageType type);

	/* Add a component to this entity, fails if there is already a component of that type attached to the entity */
	bool addComponent(Component* component);

	/* Get the pointer to a component */
	Component* getComponent(std::string componentType);

	/* Delete a component (not implemented yet) */
	bool removeComponent(std::string componentType);

	/* Check if this entity has a component of a type */
	bool hasComponent(std::string componentType);

private:
	C3DModel model_;
	C3DTransform transform_;
	std::vector<Component*> components_;

};


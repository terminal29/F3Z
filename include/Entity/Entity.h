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

	Transform getTransform();
	void setTransform(Transform transform);

	Model getModel();
	void setModel(Model model);

	void addComponent(Component* component);

	template <class C>
	C* getComponent<C>();

private:
	Model model_;
	Transform transform_;
	std::vector<Component*> components_;

};


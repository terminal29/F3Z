#include "entity/component/Transform.h"

Transform::Transform()
{
}


Transform::~Transform()
{
}

vec3f Transform::getPos() {
	return position_;
}

void Transform::setPos(vec3f position) {
	position_ = position;
}

vec3f Transform::getEulerRotation() {
	return eulerAngles_;
}

void Transform::setEulerRotation(vec3f rotation) {
	eulerAngles_ = rotation;
}

float Transform::getScale() {
	return scale_;
}

void Transform::setScale(float scale) {
	scale_ = scale;
}
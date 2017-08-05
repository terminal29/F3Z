#include <c3d++/C3DTransform.h>

C3DTransform::C3DTransform()
{
}


C3DTransform::~C3DTransform()
{
}

vec3f C3DTransform::getPos() {
	return position_;
}

void C3DTransform::setPos(vec3f position) {
	position_ = position;
}

vec3f C3DTransform::getEulerRotation() {
	return eulerAngles_;
}

void C3DTransform::setEulerRotation(vec3f rotation) {
	eulerAngles_ = rotation;
}

float C3DTransform::getScale() {
	return scale_;
}

void C3DTransform::setScale(float scale) {
	scale_ = scale;
}
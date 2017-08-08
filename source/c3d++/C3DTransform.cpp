#include <c3d++/C3DTransform.h>

C3DTransform::C3DTransform()
{
}


C3DTransform::~C3DTransform()
{
}

vec<float, 3> C3DTransform::getPos() {
	return position_;
}

void C3DTransform::setPos(vec<float, 3> position) {
	position_ = position;
}

vec<float, 3> C3DTransform::getYPR() {
	return ypr_;
}

void C3DTransform::setYPR(vec<float, 3> ypr) {
	ypr_ = ypr;
}

float C3DTransform::getScale() {
	return scale_;
}

void C3DTransform::setScale(float scale) {
	scale_ = scale;
}

// Doesnt work correctly yet
vec<float,3> C3DTransform::getForward() {
	vec<float, 3> mYPR = getYPR();
	mat<float, 4, 4> mMatYaw = rotation_matrix(rotation_quat(vec<float, 3>{0, 1, 0}, mYPR.x));
	mat<float, 4, 4> mMatPitch = rotation_matrix(rotation_quat(vec<float, 3>{1, 0, 0}, mYPR.y));
	mat<float, 4, 4> mMatRoll = rotation_matrix(rotation_quat(vec<float, 3>{0, 0, 1}, mYPR.z));
	mat<float, 4, 4> mMatYPR = mul(mMatRoll, mul(mMatPitch, mMatYaw));
	vec<float, 4> forward = { 0,0,1,0 };
	vec<float, 4> outvec = mul(mMatYPR, forward);



	return vec<float, 3>{outvec.x, outvec.y, outvec.z};
}

#pragma once

#include <library/linalg.h>
#include <cmath>
#include <citro3d.h>

using namespace linalg;

class C3DTransform
{
public:
	C3DTransform();
	~C3DTransform();

	vec<float, 3> getPos();
	void setPos(vec<float, 3> position);

	float getScale();
	void setScale(float scale);

	void setYPR(vec<float, 3> ypr);
	vec<float, 3> getYPR();

	vec<float, 3> getForward();

private:
	vec<float, 3> position_ = { 0.0f, 0.0f, 0.0f };
	vec<float, 3> ypr_ = { 0,0,0 };
	float scale_ = 1;
};


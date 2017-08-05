#pragma once
#include <Types.h>

class C3DTransform
{
public:
	C3DTransform();
	~C3DTransform();

	vec3f getPos();
	void setPos(vec3f position);

	vec3f getEulerRotation();
	void setEulerRotation(vec3f rotation);
	
	float getScale();
	void setScale(float scale);

private:
	vec3f position_ = vec3f( 0.0f, 0.0f, 0.0f );
	vec3f eulerAngles_ = vec3f(0.0f,0.0f,0.0f );
	float scale_ = 1;
};


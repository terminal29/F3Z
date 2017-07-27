#pragma once

typedef struct vec3f { float x = 0, y = 0, z = 0; } vec3f;

class Transform
{
public:
	Transform();
	~Transform();

	vec3f getPos();
	void setPos(vec3f position);

	vec3f getEulerRotation();
	void setEulerRotation(vec3f rotation);
	
	float getScale();
	void setScale(float scale);

private:
	vec3f position_;
	vec3f eulerAngles_;
	float scale_;
};


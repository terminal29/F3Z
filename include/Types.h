#pragma once

struct vec3f {
	float
		x = 0,
		y = 0,
		z = 0;
	vec3f() {};
	vec3f(float x, float y, float z) :x(x), y(y), z(z) {};
};

struct vec2f {
	float
		x = 0,
		y = 0;
	vec2f() {};
	vec2f(float x, float y) :x(x), y(y) {};
};
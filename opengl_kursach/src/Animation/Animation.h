#pragma once
#include "../func.h"

struct Animation
{
	vec3 vecToRotate = vec3(0.f);
	vec3 vecToMove = vec3(0.f);
	vec3 vecToScale = vec3(1.f);

	float angle = 0.f;

	double duration = 1.;
};
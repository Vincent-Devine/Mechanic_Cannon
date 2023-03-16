#pragma once
#include "types.hpp"

class Target
{
	// Attributes
public:
	float2 position;
	float radius;
	float point;
	int direction;
	float2 initPosition;
	float speed;

	// Methods
public:
	Target(const float2 &position, const float &radius, const float &points, const float& speed);

	void Update(const float deltaTime);
};
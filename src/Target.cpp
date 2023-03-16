#include "Target.hpp"

Target::Target(const float2 &position, const float& radius, const float& points, const float& speed)
{
	this->position = position;
	this->radius = radius;
	this->point = points;
	this->initPosition = position;
	this->speed = speed;
	direction = 1;
}

void Target::Update(const float deltaTime)
{
	// Move the target
	// update to move up and move down following the direction
	// start : move up 
	if (position.y > initPosition.y + 5.f && direction == 1)
		direction = -1;
	else if (position.y < initPosition.y - 5.f && direction == -1)
		direction = 1;

	position.y += direction * deltaTime * speed;
}
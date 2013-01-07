#include "GameFiles/ClientSide/MovingObject.h"

#define PI 3.14159265358979323846f

MovingObject::MovingObject(const unsigned int id) : WorldObject(id)
{
	this->zState = STATE_IDLE; 
	this->zEndPosition = Vector3(0,0,0);
	this->zVelocity = 100.0f;
}

float MovingObject::GetInterpolationType(const float deltaTime, const unsigned int type)
{
	float t;
	switch (type)
	{
	case IT_LINEAR:
		t = deltaTime;
		break;
	case IT_COSINE:
		t = (-cos(PI * deltaTime) * 0.5f) + 0.5f;
		break;
	case IT_ACCELERATION:
		t = pow(deltaTime, 2);
		break;
	case IT_SMOOTH_STEP:
		t = pow(deltaTime, 2) * (3 - 2 * deltaTime);
		break;
	case IT_DECELERATION:
		t = (1 - pow(1 - deltaTime, 2));
		break;
	default:
		t = deltaTime;
		break;
	}

	return t;
}

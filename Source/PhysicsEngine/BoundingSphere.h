#pragma once

#include "Vector.h"

class BoundingSphere
{
public:
	Vector3 center;
	float radius;

	BoundingSphere()
	{
		this->center = Vector3(0.0f, 0.0f, 0.0f);
		this->radius = 0.0f;
	}

	BoundingSphere(Vector3 minCorner, Vector3 maxCorner)
	{
		this->center = (maxCorner + minCorner) / 2.0f;
		this->radius = (((maxCorner - minCorner) / 2.0f)).GetLength();
	}
	virtual ~BoundingSphere() {}

};
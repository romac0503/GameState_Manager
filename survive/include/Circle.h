#pragma once

#include "StdAfx.h"

class Circle
{
public:

	ci::Vec2f position;
	float radius;
	Circle(ci::Vec2f pos, float r) : position(pos), radius(r) {}
	~Circle() {}
};

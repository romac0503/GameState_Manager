#pragma once

#include "StdAfx.h"

using namespace ci;
using namespace std;

class AABB
{
private:
	Rectf rec;

public:
	AABB(Rectf _rec):rec(_rec){}
	~AABB(){}

	bool AABBvsAABB(Rectf a, Rectf b)
	{
		return a.intersects(b);
	}

	void update();
	void draw(Vec2f pos);

	Rectf getBounds() { return rec; }
};

#include "AABB.h"

void AABB::update()
{
}

void AABB::draw(Vec2f pos)
{
	gl::pushMatrices();
	//gl::translate(-rec.x2/2, rec.y2);
	gl::drawStrokedRect(Rectf(pos.x, pos.y, pos.x + rec.x2, pos.y + rec.y2));
	gl::popMatrices();
}
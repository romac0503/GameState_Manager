#include "StdAfx.h"
#include "Treasure.h"
#include "Body.h"

using namespace ci;

Treasure::Treasure(const ci::Vec2f& position, float rotation) :
	GameObject(position, rotation)
{
	m_texture = gl::Texture(loadImage(app::loadAsset("treasure.png")));

	Vec2i halfSize(m_texture.getWidth() / 2, m_texture.getHeight() / 2);
	Rectf boundingBox(position - halfSize, position + halfSize);
	m_body = std::shared_ptr<Body>(new Body(boundingBox, 1.0f, this));
}


void Treasure::draw()
{
	gl::pushModelView();
	{
		gl::translate(position);
		gl::rotate(rotation);

		Vec2i halfSize(m_texture.getWidth() / 2, m_texture.getHeight() / 2);
		gl::draw(m_texture, Rectf(-halfSize, halfSize));
	}
	gl::popModelView();
}
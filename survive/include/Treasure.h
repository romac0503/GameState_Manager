#pragma once

#include "GameObject.h"
#include "cinder/gl/Texture.h"

class Body;

class Treasure:
	public GameObject
{
public:
	Treasure(const ci::Vec2f& position, float rotation);

	void draw() override;

private:
	std::shared_ptr<Body> m_body;
	ci::gl::Texture m_texture;
};
#pragma once

#include "GameObject.h"
#include "cinder/app/App.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

class Background : public GameObject
{
private:
	ci::gl::Texture m_texture;

public:
	Background(std::string fileName, const ci::Vec2f& pos, float rot);
	virtual ~Background();

	virtual void update(float delta);
	virtual void draw();
};

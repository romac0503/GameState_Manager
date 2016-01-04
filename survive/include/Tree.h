#pragma once

#include "GameObject.h"
#include "cinder/app/App.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

class Body;

class Tree : public GameObject
{
private: 
	ci::gl::Texture m_texture;
	std::shared_ptr<Body> m_body;

public:
	Tree(std::string fileName, const ci::Vec2f& pos, float rot);
	virtual ~Tree();

	virtual void update(float delta);
	virtual void draw();
};

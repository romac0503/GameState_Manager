#pragma once

#include "GameObject.h"
#include "StdAfx.h"
#include "AnimatedTexture.h"

class Body;

class Tree : public GameObject
{
private: 
	//ci::gl::Texture m_texture;
	std::shared_ptr<AnimatedTexture> m_texture;
	std::shared_ptr<Body> m_body;

public:
	Tree(std::string fileName, const ci::Vec2f& pos, float rot);
	virtual ~Tree();

	virtual void update(float delta);
	virtual void draw();
};

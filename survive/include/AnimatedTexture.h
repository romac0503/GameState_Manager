#pragma once

#include "StdAfx.h"
#include "cinder\CinderAssert.h"
#include <string>

class AnimatedTexture
{
private:

	struct Animation
	{
		int startIdx;
		int endIdx;
		bool m_flipped;
	};

	int m_frameRate;
	int m_frameCount;
	int m_spritesPerRow;
	int m_currFrame;
	float m_frameDelta;
	ci::Vec2f m_frameSize;
	ci::gl::Texture m_texture;
	Animation m_currentAnimation;

	std::map<std::string, Animation> animations;

	ci::Area calcSpriteArea(int playerIdx);
	void calcFrameCount();

public:
	AnimatedTexture(std::string fileName, ci::Vec2f frameSize, int frameRate);
	~AnimatedTexture();

	void addAnimation(std::string animationName, int startIdx, int endIdx, bool flipped);
	void setAnimation(const std::string& animation);
	ci::Vec2f getFrameSize();
	ci::Vec2f getSpriteSize();

	void update(float delta);
	void draw(ci::Vec2f position, float rotation, int playerIdx);

};


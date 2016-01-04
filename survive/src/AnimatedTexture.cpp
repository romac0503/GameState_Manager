#include "AnimatedTexture.h"

AnimatedTexture::AnimatedTexture(std::string fileName, ci::Vec2f frameSize, int frameRate) :
	m_frameSize(frameSize),
	m_frameRate(frameRate),
	m_frameDelta(0)
{
	try
	{
		m_texture = ci::gl::Texture(ci::loadImage(ci::app::loadAsset(fileName)));
	}
	catch (...)
	{
		std::cout << "image couldn't be found!" << std::endl;
	}
}

AnimatedTexture::~AnimatedTexture()
{

}

void AnimatedTexture::addAnimation(std::string animationName, int startIdx, int endIdx, bool flipped)
{
	Animation animation;
	animation.startIdx = startIdx;
	animation.endIdx = endIdx;
	animation.m_flipped = flipped;
	m_frameDelta = 0;

	animations[animationName] = animation;
}

void AnimatedTexture::setAnimation(const std::string& animation)
{
	auto it = animations.find(animation);
	bool found = false;

	for (auto& it: animations)
	{
		if (it.first == animation)
		{
			m_currentAnimation = it.second;
			if (m_currFrame < it.second.startIdx || m_currFrame > it.second.endIdx)
			{
				m_currFrame = it.second.startIdx - 1;
				if (m_currFrame < 0 || m_currFrame > m_currentAnimation.endIdx)		// condition is necessary because of unwanted Sprites in the animation
				{
					m_currFrame = m_currentAnimation.startIdx;
				}
			}
			found = true;
			break;
		}
	}

	if (!found)
	{
		std::cout << "animation " << animation << " not found!" << std::endl;
	}
}

ci::Vec2f AnimatedTexture::getFrameSize()
{
	return m_frameSize;
}

void AnimatedTexture::update(float delta)
{
	m_frameDelta += delta;
	int numOfSpritesForAnimation = m_currentAnimation.endIdx + 1;

	float frameDuration = 1.0f / m_frameRate;

	if (m_frameDelta > frameDuration)
	{
		if (m_currFrame >= m_currentAnimation.startIdx || m_currFrame <= m_currentAnimation.endIdx)
		{
			m_currFrame = (m_currFrame + 1) % numOfSpritesForAnimation;
			if (m_currFrame < m_currentAnimation.startIdx || m_currFrame > m_currentAnimation.endIdx)
			{
				m_currFrame = m_currentAnimation.startIdx;
			}
		}
		m_frameDelta -= frameDuration;
	}
	
}

void AnimatedTexture::draw(ci::Vec2f pos, float rotation, int playerIdx)
{
	cinder::gl::pushMatrices();
	
	ci::gl::translate(pos);
	ci::gl::rotate(rotation);

	if (m_currentAnimation.m_flipped)
	{
		ci::gl::scale(cinder::Vec2f(-1, 1));
	}

	// set origin to bottom-center.
	ci::gl::translate(-m_frameSize.x / 2, -m_frameSize.y + (m_frameSize.y/2) - 10);		// had to adjust the position to fit inside the boundingbox

	ci::Area area = calcSpriteArea(playerIdx);
	ci::gl::draw(m_texture, area, cinder::Rectf(0,0, m_frameSize.x, m_frameSize.y));

	cinder::gl::popMatrices();
}

void AnimatedTexture::calcFrameCount()
{
	int frameCountX = m_texture.getWidth() / m_frameSize.x;
	int frameCountY = m_texture.getHeight() / m_frameSize.y;

	m_frameCount = frameCountX * frameCountY;
}

ci::Vec2f AnimatedTexture::getSpriteSize()
{
	float length = m_texture.getWidth() / m_frameSize.x;
	float height = m_texture.getHeight() / m_frameSize.y;

	return ci::Vec2f(length, height);
}

ci::Area AnimatedTexture::calcSpriteArea(int playerIdx)
{
	//std::cout << "Index = " << playerIdx << std::endl;
	int offset = playerIdx * m_frameSize.y;

	int posX = (m_currFrame % m_spritesPerRow) * m_frameSize.x;
	int posY = ((m_currFrame / m_spritesPerRow) * m_frameSize.y) * playerIdx;

	//std::cout << "playerIndex = " << playerIdx  << " pos Y = " << posY << std::endl;

	return ci::Area(posX, offset + posY, posX + m_frameSize.x, offset + posY + m_frameSize.y);
}


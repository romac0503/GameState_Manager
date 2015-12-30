#include "Background.h"

Background::Background(std::string fileName, const ci::Vec2f& pos, float rot):GameObject(pos, rot)
{
	try
	{
		m_texture = ci::gl::Texture(ci::loadImage(ci::app::loadAsset(fileName)));
		m_texture.setWrap(GL_REPEAT, GL_REPEAT);
	}
	catch (...)
	{
		std::cout << "File could not be loaded!" << std::endl;
	}
}

Background::~Background()
{

}

void Background::draw()
{
	float w = m_texture.getWidth();
	float h = m_texture.getHeight();
	cinder::gl::pushMatrices();
	
	ci::gl::translate(position);
	ci::gl::rotate(rotation);

	// set origin to left-upper corner.
	ci::gl::translate(-ci::app::getWindowWidth() / 2, 0);
	ci::gl::draw(m_texture, ci::Area(0, 0, w * ci::app::getWindowWidth(), h * ci::app::getWindowHeight()), cinder::Rectf(0, 0, w * ci::app::getWindowWidth(), h * ci::app::getWindowHeight()));

	cinder::gl::popMatrices();
}

void Background::update(float delta)
{

}
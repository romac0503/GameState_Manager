#include "Background.h"

Background::Background(std::string fileName, const ci::Vec2f& pos, float rot):GameObject(pos, rot)
{
	try
	{
		m_texture = ci::gl::Texture(ci::loadImage(ci::app::loadAsset(fileName)));
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

}

void Background::update(float delta)
{

}
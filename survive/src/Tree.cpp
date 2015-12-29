#include "Tree.h"

Tree::Tree(std::string fileName, const ci::Vec2f& pos, float rot) : GameObject(pos, rot)
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

Tree::~Tree()
{

}

void Tree::update(float delta)
{

}

void Tree::draw()
{
	cinder::gl::pushMatrices();

	ci::gl::translate(position);
	ci::gl::rotate(rotation);

	// set origin to bottom-center.
	ci::gl::translate(-m_texture.getSize().x / 2, m_texture.getSize().y);
	ci::gl::draw(m_texture, ci::Area(0, 0, m_texture.getSize().x, m_texture.getSize().y), cinder::Rectf(0, 0, m_texture.getSize().x, m_texture.getSize().y));

	cinder::gl::popMatrices();
}
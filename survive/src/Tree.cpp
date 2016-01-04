#include "Tree.h"

Tree::Tree(std::string fileName, const ci::Vec2f& pos, float rot) : GameObject(pos, rot)
{
	try
	{
		m_texture = ci::gl::Texture(ci::loadImage(ci::app::loadAsset(fileName)));
		/*Rectf box(0, 0, m_texture.getWidth(), m_texture.getHeight());
		collisionBox = new AABB(box);*/
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
	ci::gl::pushMatrices();

	ci::gl::translate(position);
	ci::gl::rotate(rotation);

	// set origin to bottom-center.
	
	ci::gl::draw(m_texture, ci::Area(0, 0, m_texture.getSize().x, m_texture.getSize().y), cinder::Rectf(position.x, position.y,position.x + m_texture.getSize().x, position.y + m_texture.getSize().y));
	//collisionBox->draw(position);

	ci::gl::popMatrices();
}
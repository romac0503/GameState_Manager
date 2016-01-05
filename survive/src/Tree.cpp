#include "Tree.h"
#include "Body.h"

using namespace ci;

Tree::Tree(std::string fileName, const ci::Vec2f& pos, float rot) : GameObject(pos, rot)
{
	try
	{
		//m_texture = ci::gl::Texture(ci::loadImage(ci::app::loadAsset(fileName)));
		m_texture = std::shared_ptr<AnimatedTexture>(new AnimatedTexture(fileName, ci::Vec2f(110.0f, 96.0f), 0));
		Rectf bounds(position - Vec2f(55, 55), position + Vec2f(55, 40));
		m_body = std::shared_ptr<Body>(new Body(bounds, 0.0f, this));
		
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
	gl::pushModelView();
	{
		m_texture->draw(position, rotation, 0);
	}
	gl::popModelView();

	//ci::gl::pushMatrices();

	///*ci::gl::translate(position);
	//ci::gl::rotate(rotation);*/

	//ci::gl::draw(m_texture, ci::Area(0, 0, m_texture.getSize().x, m_texture.getSize().y), cinder::Rectf(position.x, position.y, position.x + m_texture.getSize().x, position.y + m_texture.getSize().y));

	//ci::gl::popMatrices();
}
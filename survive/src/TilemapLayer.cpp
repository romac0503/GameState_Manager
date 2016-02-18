#include "TilemapLayer.h"

using namespace ci;

TilemapLayer::TilemapLayer(const ci::Vec2f& position, const ci::Area& source, ci::gl::TextureRef texture, float rotation, ci::Vec2i tilesize):
	GameObject(position, rotation),
	m_texture(texture), 
	m_source(source), 
	m_tileSize(tilesize)
{
}

void TilemapLayer::draw()
{
	/*Vec3f eye = m_camera->getEyePoint();

	float left, top, right, bottom, nearZ, farZ;
	m_camera->getFrustum(&left, &top, &right, &bottom, &nearZ, &farZ);
	Rectf viewport(eye.x + left, eye.y + top, eye.x + right, eye.y + bottom);
*/
	// only render tiles within my viewport
	gl::draw(m_texture, m_source, Rectf(position.x - m_tileSize.x / 2, position.y - m_tileSize.y / 2, position.x + m_tileSize.x / 2, position.y + m_tileSize.y / 2));

}
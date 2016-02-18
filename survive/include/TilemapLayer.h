#include "GameObject.h"
#include "StdAfx.h"
#include "NLTmxMap.h"
#include "cinder\Camera.h"

class TilemapLayer :
	public GameObject
{
public:
	typedef std::map<std::string, ci::gl::TextureRef> TextureMap;

	/// \position the offset of the tilemap
	/// \texturemap a map of all tileset textures as shared pointer
	/// \layer the NLTmxMapLayer to be drawn.
	TilemapLayer(const ci::Vec2f& position, const ci::Area& source, ci::gl::TextureRef texture, float _rotation, ci::Vec2i tilesize);

	/// render the layer
	void draw() override;

	/// set the active camera to only render the current viewport
	/*void setCamera(std::shared_ptr<ci::CameraOrtho> camera)
	{
		m_camera = camera;
	}*/

private:
	ci::gl::TextureRef m_texture;
	ci::Area m_source;
	ci::Vec2i m_tileSize;
};
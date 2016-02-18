#include "StdAfx.h"
#include "MainState.h"
#include "cinder/gl/gl.h"
#include "SurviveApp.h"
#include "StdAfx.h"
#include "GameStateManager.h"
#include "InputManager.h"

#include "Player.h"
#include "Tree.h"
#include "Background.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void MainState::init()
{
	physics = Physics::getInstance();
	loadMap("map1.tmx", ci::Vec2f(0.0f, 0.0f));

	InputManager::getInstance()->bind("left", ci::app::KeyEvent::KEY_a, 0);
	InputManager::getInstance()->bind("right", ci::app::KeyEvent::KEY_d, 0);
	InputManager::getInstance()->bind("up", ci::app::KeyEvent::KEY_w, 0);
	InputManager::getInstance()->bind("down", ci::app::KeyEvent::KEY_s, 0);
	InputManager::getInstance()->bind("digging", ci::app::KeyEvent::KEY_y, 0);
	InputManager::getInstance()->bind("fight", ci::app::KeyEvent::KEY_SPACE, 0);

	InputManager::getInstance()->bind("left", ci::app::KeyEvent::KEY_LEFT, 1);
	InputManager::getInstance()->bind("right", ci::app::KeyEvent::KEY_RIGHT, 1);
	InputManager::getInstance()->bind("up", ci::app::KeyEvent::KEY_UP, 1);
	InputManager::getInstance()->bind("down", ci::app::KeyEvent::KEY_DOWN, 1);
	InputManager::getInstance()->bind("digging", ci::app::KeyEvent::KEY_COMMA, 1);
	InputManager::getInstance()->bind("fight", ci::app::KeyEvent::KEY_l, 1);
	
	//TODO: create Player object and add it to gameObjects
	player1 = std::shared_ptr<Player>(new Player(ci::Vec2f(10, 100), 0, 0));

	//gameObjects.push_back(GameObjectPtr(new Background("background.png", ci::Vec2f(0,0),0)));
	gameObjects.push_back(player1);
	//gameObjects.push_back(GameObjectPtr(new Player(ci::Vec2f(100, 100), 0, 1)));
	//gameObjects.push_back(GameObjectPtr(new Tree("tree.png", ci::Vec2f(50, 200), 0)));

	camera.setOrtho(-app::getWindowWidth() / 2, app::getWindowWidth() / 2, app::getWindowHeight(), 0, -100, 100);
	camera.lookAt(Vec3f(0, 0, 1), Vec3f(0, 0, 0));
}

void MainState::exit()
{	
	// destroy game objects
	gameObjects.clear();

	InputManager::getInstance()->unbind("left", 0);
	InputManager::getInstance()->unbind("right", 0);
	InputManager::getInstance()->unbind("up", 0);
	InputManager::getInstance()->unbind("down", 0);
	InputManager::getInstance()->unbind("digging", 0);
	InputManager::getInstance()->unbind("fight", 0);

	InputManager::getInstance()->unbind("left", 1);
	InputManager::getInstance()->unbind("right", 1);
	InputManager::getInstance()->unbind("up", 1);
	InputManager::getInstance()->unbind("down", 1);
	InputManager::getInstance()->unbind("digging", 1);
	InputManager::getInstance()->unbind("fight", 1);
}

void MainState::loadMap(const string& filename, const Vec2f& offset)
{
	ci::DataSourceRef dataSource = ci::app::loadAsset(filename);

	// convert DataSource to char* bufString
	Buffer buf = dataSource->getBuffer();
	size_t dataSize = buf.getDataSize();
	char* bufString = new char[dataSize + 1];
	memcpy(bufString, buf.getData(), buf.getDataSize());
	bufString[dataSize] = 0;

	// now lets load a NLTmxMap
	NLTmxMap* tilemap = NLLoadTmxMap(bufString);

	app::console() << "Load tilemap with size: " << tilemap->width << ", " << tilemap->height << " and tilesize: " << tilemap->tileWidth << ", " << tilemap->tileHeight << std::endl;

	// store tileset textures in map
	std::map<std::string, gl::TextureRef> tilesetTexture;

	// load textures for every tileset
	for (auto tileset : tilemap->tilesets)
	{
		app::console() << "Load tileset: " << tileset->name << " width filename: " << tileset->filename << " and tilesize: " << tileset->tileWidth << ", " << tileset->tileHeight << std::endl;
		gl::TextureRef texture = ci::gl::TextureRef(new gl::Texture(ci::loadImage(ci::app::loadAsset(tileset->filename))));
		tilesetTexture[tileset->name] = texture;
	}

	// go through all layers
	for (int layerIdx = 0; layerIdx < (int)tilemap->layers.size(); layerIdx++)
	{
		NLTmxMapLayer* layer = tilemap->layers[layerIdx];
		app::console() << "Load layer: " << layer->name << " with width: " << layer->width << " and height: " << layer->height << std::endl;

		int size = layer->width * layer->height;

		// go over all elements in the layer
		for (int i = 0; i < size; i++)
		{
			int grid = layer->data[i];

			if (grid == 0)
			{
				// 0 means there is no tile at this grid position,
				continue;
			}

			// get tileset and tileset texture
			NLTmxMapTileset* tileset = tilemap->getTilesetForGrid(grid);
			Vec2i tileSize(tilemap->tileWidth, tilemap->tileHeight);
			gl::TextureRef texture = tilesetTexture[tileset->name];

			// horizontal tile count in tileset texture
			int tileCountX = texture->getWidth() / tileSize.x;

			// calcualte position of tile
			Vec2f position;
			position.x = (i % layer->width) * (float)tileSize.x;
			position.y = (i / layer->width) * (float)tileSize.y;
			//position += offset;

			// calculate 2d idx of tile in tileset texture
			int idx = grid - tileset->firstGid;		
			int idxX = idx % tileCountX;
			int idxY = idx / tileCountX;

			// caclucalte source area of tile in tileset texture
			Area source(idxX * tileSize.x, idxY * tileSize.y, (idxX + 1) * tileSize.x, (idxY + 1) * tileSize.y);

			// TODO create tile
			std::cout << "position.x = " << position.x << " position.y = " << position.y << std::endl;
			
			m_tile = shared_ptr<TilemapLayer>(new TilemapLayer(position, source, texture, 0, tileSize));
			std::cout << layer->name << "\n";
			if (layer->name == "Coll")
			{
				Rectf bounds(position, position + Vec2i(40, 40));

				shared_ptr<GameObject> blockingObj = shared_ptr<GameObject>(new GameObject(ci::Vec2f(position), 0));
				gameObjects.push_back(blockingObj);
			}
			
			gameObjects.push_back(m_tile);
		}
	}

	// go through all object layers
	//for (auto group : tilemap->groups)
	//{
	//	// go over all objects per layer
	//	for (auto object : group->objects)
	//	{
	//		Vec2i position(object->x, object->y);
	//		position += offset;

	//		Rectf bounds(position, position + Vec2i(object->width, object->height));

	//		// TODO create object
	//	}
	//}
}

void MainState::update(float delta)
{
	// return to MainState when ESC is released.
	if (InputManager::getInstance()->isKeyReleased(ci::app::KeyEvent::KEY_ESCAPE))
	{
		manager->setState("MenuState");
	}

	physics->update(delta);

	for (auto gameObject : gameObjects)
	{
		gameObject->update(delta);
	}

	// Camera stuff -> as a Reference from another project
	ci::Vec3f newLookAt(player1->getPosition().x, player1->getPosition().y - 300, 0);
	ci::Vec3f camOffset = ci::Vec3f(0, 0, 1);

	m_cameraLockAt = m_cameraLockAt.lerp(0.1f, newLookAt);
	camera.lookAt(m_cameraLockAt + camOffset, m_cameraLockAt);
}

void MainState::draw()
{
	gl::pushMatrices();
	gl::setMatrices(camera);

	for (auto gameObject : gameObjects)
	{
		gameObject->draw();
		physics->draw();
	}

	gl::popMatrices();
}
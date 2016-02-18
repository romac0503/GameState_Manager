#pragma once

#include "GameState.h"
#include "cinder\Camera.h"
#include <vector>
#include "Physics.h"
#include "Body.h"
#include "NLTmxMap.h"
#include "TilemapLayer.h"
#include "Player.h"

class GameObject;
class Player;

class MainState :
	public GameState
{
public:
	MainState(GameStateManager* _manager, SurviveApp* _app) :
		GameState(_manager, _app)
	{}

	virtual void init();
	virtual void exit();

	virtual void update(float delta);
	virtual void draw();

	void loadMap(const string& filename, const ci::Vec2f& offset);

private:
	typedef std::shared_ptr<GameObject> GameObjectPtr;
	typedef std::vector<GameObjectPtr> GameObjectList;

	GameObjectList gameObjects;
	ci::CameraOrtho camera;
	ci::Vec3f m_cameraLockAt;
	shared_ptr<TilemapLayer> m_tile;
	
	shared_ptr<Player> player1;		// pointer to Player1 -> needed to move the camera with the player

	Physics* physics;
};
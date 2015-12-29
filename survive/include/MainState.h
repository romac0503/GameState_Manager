#pragma once

#include "GameState.h"
#include "cinder\Camera.h"
#include <vector>

class GameObject;

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

private:
	typedef std::shared_ptr<GameObject> GameObjectPtr;
	typedef std::vector<GameObjectPtr> GameObjectList;

	GameObjectList gameObjects;
	ci::CameraOrtho camera;
};
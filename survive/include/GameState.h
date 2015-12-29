#pragma once

#include "cinder/app/AppNative.h"

class GameStateManager;
class SurviveApp;

class GameState
{
public:
	GameState(GameStateManager* _manager, SurviveApp* _app) :
		manager(_manager),
		app(_app)
	{}

	virtual ~GameState() {}

	/// called when game state is entered.
	virtual void init() {};

	/// called when game state is left.
	virtual void exit() {};

	virtual void mouseDown(ci::app::MouseEvent event) {}
	virtual void keyDown(ci::app::KeyEvent event) {}
	virtual void keyUp(ci::app::KeyEvent event) {}

	virtual void update(float delta) = 0;
	virtual void draw() = 0;

protected:
	GameStateManager* manager;
	SurviveApp* app;
};
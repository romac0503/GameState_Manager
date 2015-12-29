#pragma once

#include "GameState.h"

class MenuState :
	public GameState
{
public:
	MenuState(GameStateManager* _manager, SurviveApp* _app) :
		GameState(_manager, _app)
	{}

	virtual void keyUp(ci::app::KeyEvent event);

	virtual void update(float delta);
	virtual void draw();
};
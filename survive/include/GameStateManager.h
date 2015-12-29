#pragma once

#include <map>
#include <string>
#include "cinder/app/AppNative.h"

class GameState;

class GameStateManager
{
public:
	GameStateManager();

	/// registers a new GameState with the given name.
	void registerState(const std::string& name, GameState* state);

	/// Changes to the given state
	void setState(const std::string& stateName);

	void mouseDown(ci::app::MouseEvent event);
	void keyDown(ci::app::KeyEvent event);
	void keyUp(ci::app::KeyEvent event);

	void update(float delta);
	void draw();

private:
	GameState* findState(const std::string& stateName);
	void changeState(GameState* stateName);

	std::map<std::string, GameState*> states;
	GameState* currentState;
	GameState* futureState;
};
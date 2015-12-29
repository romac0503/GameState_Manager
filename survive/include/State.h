#pragma once

#include "StdAfx.h"

// forward declaration
class StateManager;

class State
{
public:
	State(StateManager* _manager) : manager(_manager){}

	virtual ~State(){}

	// called when state is entered.
	virtual void init(StateManager* manager) {}

	// called when state is left.
	virtual void exit() {}

	virtual void update(float delta) = 0;
	virtual void draw(Sprite& sprite, cinder::Vec2f& pos, int framesPerSec, bool isFlipped) = 0;

	virtual void keyDown(ci::app::KeyEvent event, cinder::Vec2f& pos, cinder::Vec2f& velocity) {}
	virtual void keyUp(ci::app::KeyEvent event) {}

protected:
	StateManager* manager;

};

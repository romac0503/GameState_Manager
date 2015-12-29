#include "StdAfx.h"
#include "MenuState.h"

#include "cinder/gl/gl.h"
#include "SurviveApp.h"
#include "GameStateManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void MenuState::keyUp(ci::app::KeyEvent event)
{
	//TODO: Change to MainState on key release.
	if (event.getChar() != ' ' || event.getChar() == ' ')
	{
		manager->setState("MainState");
	}
}

void MenuState::update(float delta)
{
}

void MenuState::draw()
{
	app->drawText("Press any key to continue!", app->getWindowSize() / 2, true);
}
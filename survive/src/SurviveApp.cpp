#include "StdAfx.h"
#include "SurviveApp.h"
#include "cinder/gl/gl.h"

#include "GameStateManager.h"
#include "InputManager.h"

#include "MenuState.h"
#include "MainState.h"
#include "InputManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;



void SurviveApp::setup()
{
	// following 3 lines are for opening a consol window, as this is not standard for a cinder app
	FILE* f;
	AllocConsole();
	freopen_s(&f, "CON", "w", stdout);

	// Initialize engine 
	inputManager = InputManager::getInstance();
	gameStateManager = shared_ptr<GameStateManager>(new GameStateManager());

	// create font texture for text drawing
	Font font = Font("Consolas", 16);
	textureFont = gl::TextureFont::create(font);


	// Create game states
	MenuState* menuState = new MenuState(gameStateManager.get(), this);
	gameStateManager->registerState("MenuState", menuState);

	MainState* mainState = new MainState(gameStateManager.get(), this);
	gameStateManager->registerState("MainState", mainState);
	
	// Jump to first game state
	gameStateManager->setState("MenuState");
	lastTime = ci::app::getElapsedSeconds();
}

void SurviveApp::shutdown()
{
	// nothing to do yet.
}

void SurviveApp::mouseDown( MouseEvent event )
{
	inputManager->mouseDown(event);
	gameStateManager->mouseDown(event);
}

void SurviveApp::keyDown( KeyEvent event)
{
	inputManager->keyDown(event);
	gameStateManager->keyDown(event);
}

void SurviveApp::keyUp( KeyEvent event)
{
	inputManager->keyUp(event);
	gameStateManager->keyUp(event);
}

void SurviveApp::update()
{
	double  time = ci::app::getElapsedSeconds();
	float deltaTime = min((float) (time - lastTime), 1.0f);
	lastTime = time;

	// always update input first
	inputManager->update();

	//// update game physic
	//physics->update(deltaTime);

	// update game state
	gameStateManager->update(deltaTime);
}

void SurviveApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

	// enable transparency
	gl::enableAlphaTest();

	// draw game state
	gameStateManager->draw();
}

void SurviveApp::drawText(const std::string& text, const ci::Vec2f& position, bool centered)
{
	Vec2f offset;

	if (centered)
	{
		offset = -textureFont->measureString(text) / 2.0f;
	}
	
	textureFont->drawString(text, position + offset);
}

CINDER_APP_NATIVE( SurviveApp, RendererGl )

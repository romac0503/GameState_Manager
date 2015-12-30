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
	gameObjects.push_back(GameObjectPtr(new Background("background.png", ci::Vec2f(0,0),0)));
	gameObjects.push_back(GameObjectPtr(new Player(ci::Vec2f(10, 10), 0, 0)));
	gameObjects.push_back(GameObjectPtr(new Player(ci::Vec2f(300, 100), 0, 1)));
	gameObjects.push_back(GameObjectPtr(new Tree("tree.png", ci::Vec2f(100, 200), 0)));

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

void MainState::update(float delta)
{
	// return to MainState when ESC is released.
	if (InputManager::getInstance()->isKeyReleased(ci::app::KeyEvent::KEY_ESCAPE))
	{
		manager->setState("MenuState");
	}

	for (auto gameObject : gameObjects)
	{
		gameObject->update(delta);
	}
}

void MainState::draw()
{
	gl::pushMatrices();
	gl::setMatrices(camera);

	for (auto gameObject : gameObjects)
	{
		gameObject->draw();
	}

	gl::popMatrices();
}
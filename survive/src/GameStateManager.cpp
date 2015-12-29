#include "StdAfx.h"
#include "GameStateManager.h"
#include "GameState.h"
#include "cinder/CinderAssert.h"

GameStateManager::GameStateManager():
	currentState(nullptr)
{}

void GameStateManager::registerState(const std::string& name, GameState* state)
{
	states[name] = state;
}

void GameStateManager::setState(const std::string& stateName)
{
	GameState* state = findState(stateName);
	CI_ASSERT_MSG(state != nullptr, "State could not be found");

	futureState = state;
}

void GameStateManager::changeState(GameState* state)
{
	if (state != currentState)
	{
		if (currentState != nullptr)
		{
			currentState->exit();
		}

		currentState = state;

		if (currentState != nullptr)
		{
			currentState->init();
		}
	}
}

void GameStateManager::mouseDown(ci::app::MouseEvent event)
{
	if (currentState != nullptr)
	{
		currentState->mouseDown(event);
	}
}

void GameStateManager::keyDown(ci::app::KeyEvent event)
{
	if (currentState != nullptr)
	{
		currentState->keyDown(event);
	}
}

void GameStateManager::keyUp(ci::app::KeyEvent event)
{
	if (currentState != nullptr)
	{
		currentState->keyUp(event);
	}
}

void GameStateManager::update(float delta)
{
	if (futureState != nullptr)
	{
		changeState(futureState);
		futureState = nullptr;
	}


	if (currentState != nullptr)
	{
		currentState->update(delta);
	}
}

void GameStateManager::draw()
{
	if (currentState != nullptr)
	{
		currentState->draw();
	}
}

GameState* GameStateManager::findState(const std::string& stateName)
{
	auto state = states.find(stateName);

	if (state != states.end())
	{
		return state->second;
	}

	return nullptr;
}
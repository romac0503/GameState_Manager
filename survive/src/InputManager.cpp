#include "StdAfx.h"
#include "InputManager.h"
#include <stdio.h>


InputManager* InputManager::getInstance() 
{
	static InputManager instance;
	return &instance;
}

InputManager::InputManager()
{
	memset(&lastFrame, 0, sizeof(lastFrame));
	memset(&currentFrame, 0, sizeof(currentFrame));
	memset(&eventFrame, 0, sizeof(eventFrame));
	memset(&controllers, 0, sizeof(ControllerState) * PLAYER_COUNT);
}

void InputManager::update()
{
	memcpy(&lastFrame, &currentFrame, sizeof(currentFrame));
	memcpy(&currentFrame, &eventFrame, sizeof(currentFrame));

	updateControllerState();
}

void InputManager::updateControllerState()
{
	DWORD dwResult;
	for (DWORD i = 0; i < PLAYER_COUNT; i++)
	{
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &controllers[i].state);

		if (dwResult == ERROR_SUCCESS)
			controllers[i].connected = true;
		else
			controllers[i].connected = false;
	}
}

void InputManager::mouseDown(ci::app::MouseEvent event)
{
}

void InputManager::keyDown(ci::app::KeyEvent event)
{
	eventFrame.keys[event.getCode()] = true;
}
void InputManager::keyUp(ci::app::KeyEvent event)
{
	eventFrame.keys[event.getCode()] = false;
}

void InputManager::bind(const std::string& action, int keyCode, int playerIdx)
{
	CI_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");
	actionBinding[playerIdx][action] = keyCode;
}

void InputManager::unbind(const std::string& action, int playerIdx)
{
	CI_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");
	actionBinding[playerIdx].erase(action);
}

float InputManager::getLeftXAxis(int playerIdx)
{
	CI_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");

	if (controllers[playerIdx].connected)
	{
		const float INPUT_MAX = 0x7FFF;	// max thumstick value
		return controllers[playerIdx].state.Gamepad.sThumbLX / INPUT_MAX;
	}

	return 0.0f;
}

float InputManager::getLeftYAxis(int playerIdx)
{
	CI_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");

	if (controllers[playerIdx].connected)
	{
		const float INPUT_MAX = 0x7FFF;	// max thumstick value
		return controllers[playerIdx].state.Gamepad.sThumbLY / INPUT_MAX;
	}

	return 0.0f;
}

float InputManager::getRightXAxis(int playerIdx)
{
	CI_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");

	if (controllers[playerIdx].connected)
	{
		const float INPUT_MAX = 0x7FFF;	// max thumstick value
		return controllers[playerIdx].state.Gamepad.sThumbRX / INPUT_MAX;
	}

	return 0.0f;
}

float InputManager::getRightYAxis(int playerIdx)
{
	CI_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");

	if (controllers[playerIdx].connected)
	{
		const float INPUT_MAX = 0x7FFF;	// max thumstick value
		return controllers[playerIdx].state.Gamepad.sThumbRY / INPUT_MAX;
	}

	return 0.0f;
}

bool InputManager::isButtonDown(WORD button, int playerIdx)
{
	CI_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");

	return controllers[playerIdx].state.Gamepad.wButtons & button;
}

XINPUT_STATE InputManager::getInputState(int playerIdx)
{
	CI_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");

	return controllers[playerIdx].state;
}


int InputManager::getKeyForAction(const std::string& action, int playerIdx)
{
	CI_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");

	auto it = actionBinding[playerIdx].find(action);

	if (it != actionBinding[playerIdx].end())
	{
		return it->second;
	}

	return 0;
}

bool InputManager::isKeyDown(const std::string& action, int player)
{
	return isKeyDown(getKeyForAction(action, player));
}

bool InputManager::isKeyUp(const std::string& action, int player)
{
	return isKeyUp(getKeyForAction(action, player));
}

bool InputManager::isKeyPressed(const std::string& action, int player)
{
	return isKeyPressed(getKeyForAction(action, player));
}

bool InputManager::isKeyReleased(const std::string& action, int player)
{
	return isKeyReleased(getKeyForAction(action, player));
}


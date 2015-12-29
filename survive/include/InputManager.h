#pragma once

#include "cinder/app/App.h"
#include "cinder/CinderAssert.h"
#include <string>
#include <map>

//-------------------------------------------------------------------------------------
// read controller input using XInput.
// based on the official XInput Win32 sample:
// https://code.msdn.microsoft.com/windowsdesktop/XInput-Win32-Samples-cc25ce24

#ifdef USE_DIRECTX_SDK
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\include\xinput.h>
#pragma comment(lib,"xinput.lib")
#elif (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib,"xinput9_1_0.lib")
#endif
//-------------------------------------------------------------------------------------

/**
 * The InputManager caches the key events for easy pulling during the game update.
 * For more flexibility a key button can be bound to actions for simple input mapping.
 * 
 * Usage:
 *   Call keyDown/keyUp of this class when event is triggered in GameApp
 *   Call update() of this class at the beginning of the update in the GameApp
 *
 * Pull key event during game update:
 *   if (InputManager::getSingleton()->isKeyReleased(ci::app::KeyEvent::KEY_SPACE))
 *     // doStuff()
 *   }
 *
 * Use Action mapping:
 *   
 *   void init() {
 *     InputManager::getSingleton()->bind("left", ci::app::KeyEvent::KEY_LEFT, 0);
 *   }
 *
 *   void exit() {
 *     InputManager::getSingleton()->unbind("left", 0);
 *   }
 *
 *   void update() {
 *     if (InputManager::getSingleton()->isKeyReleased("left", 0)) {
 *       // doStuff()
 *     }
 */
class InputManager
{
public:
	static InputManager* getInstance();

	/// Must be called at the beginning of the game update
	void update();

	void mouseDown(ci::app::MouseEvent event);
	void keyDown(ci::app::KeyEvent event);
	void keyUp(ci::app::KeyEvent event);

	/// Binds an action to the given keycode and for the given player
	/// \param action the name of the action (eg. left, jump, ...)
	/// \param keyCode the keycode of the action
	/// \param playerIdx the idx of the player
	void bind(const std::string& action, int keyCode, int playerIdx = 0);

	/// Unbinds an action.
	void unbind(const std::string& action, int playerIdx = 0);

	/// Returns true if the key button is currently down.
	bool isKeyDown(int keyCode)
	{
		CI_ASSERT_MSG(keyCode < ci::app::KeyEvent::KEY_LAST, "KeyCode out of bounds");
		return currentFrame.keys[keyCode];
	}

	/// Returns true if the key button is currently up.
	bool isKeyUp(int keyCode)
	{
		CI_ASSERT_MSG(keyCode < ci::app::KeyEvent::KEY_LAST, "KeyCode out of bounds");
		return !currentFrame.keys[keyCode];
	}

	/// Returns true if the key button has been pressed.
	bool isKeyPressed(int keyCode)
	{
		CI_ASSERT_MSG(keyCode < ci::app::KeyEvent::KEY_LAST, "KeyCode out of bounds");
		return currentFrame.keys[keyCode] && !lastFrame.keys[keyCode];
	}

	/// Returns true if the key button has been released.
	bool isKeyReleased(int keyCode)
	{
		CI_ASSERT_MSG(keyCode < ci::app::KeyEvent::KEY_LAST, "KeyCode out of bounds");
		return !currentFrame.keys[keyCode] && lastFrame.keys[keyCode];
	}

	/// Returns true if the button for the given Action is currently down.
	bool isKeyDown(const std::string& action, int playerIdx = 0);

	/// Returns true if the button for the given Action is currently up.
	bool isKeyUp(const std::string& action, int playerIdx = 0);

	/// Returns true if the button for the given Action has been pressed.
	bool isKeyPressed(const std::string& action, int playerIdx = 0);

	/// Returns true if the button for the given Action has been released.
	bool isKeyReleased(const std::string& action, int playerIdx = 0);

	/// Returns the x axis value of the left thumbstick in range between -1.0f ... 1.0f without deadzone removal.
	float getLeftXAxis(int playerIdx = 0);

	/// Returns the y axis value of the left thumbstick in range between -1.0f ... 1.0f without deadzone removal.
	float getLeftYAxis(int playerIdx = 0);

	/// Returns the x axis value of the right thumbstick in range between -1.0f ... 1.0f without deadzone removal.
	float getRightXAxis(int playerIdx = 0);

	/// Returns the y axis value of the right thumbstick in range between -1.0f ... 1.0f without deadzone removal.
	float getRightYAxis(int playerIdx = 0);

	/// Returns true if a button is down on the controller.
	/// The keys of the buttons is stored in <XInput.h> as constants starting with XINPUT_GAMEPAD_*
	/// eg.: XINPUT_GAMEPAD_A defines the key for the "a" button on the controller.
	bool isButtonDown(WORD buttonKey, int playerIdx = 0);

	/// Returns the XINPUT_STATE of the controller.
	XINPUT_STATE getInputState(int playerIdx);

private:
	InputManager();
	~InputManager() {}

	InputManager(const InputManager& p);
	InputManager& operator=(InputManager const&);

	void updateControllerState();
	int getKeyForAction(const std::string& action, int player);

	struct FrameData
	{
		bool keys[ci::app::KeyEvent::KEY_LAST];
	};

	struct ControllerState
	{
		XINPUT_STATE state;
		bool connected;
	};

	
	FrameData lastFrame;
	FrameData currentFrame;
	FrameData eventFrame;

	/// maximum allowed players. Can be increaded if needed.
	static const int PLAYER_COUNT = 4;
	std::map<std::string, int> actionBinding[PLAYER_COUNT];
	ControllerState controllers[PLAYER_COUNT];
};
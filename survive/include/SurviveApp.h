#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/TextureFont.h"
#include "Physics.h"

class GameStateManager;
class InputManager;

class SurviveApp: public ci::app::AppNative 
{
public:
	/// initialize game
	void setup();

	/// uninitialize game
	void shutdown();

	/// mouse callback
	void mouseDown(ci::app::MouseEvent event);

	/// keydown callback
	void keyDown(ci::app::KeyEvent event);

	/// keyup callback
	void keyUp(ci::app::KeyEvent event);

	/// update the game
	void update();

	/// render the game
	void draw();

	/// Helper function to render text on the screen.
	void drawText(const std::string& text, const ci::Vec2f& position, bool centered = false);

private:
	std::shared_ptr<GameStateManager> gameStateManager;
	ci::gl::TextureFontRef textureFont;
	double lastTime;

	InputManager* inputManager;
	//Physics* physics;
};
#pragma once

#include "GameObject.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/App.h"
#include "cinder/ImageIo.h"
#include "AnimatedTexture.h"

class Body;

class Player :
	public GameObject
{
public:
	Player(const ci::Vec2f& _position, float _rotation, int _playerIdx);

	virtual ~Player();

	virtual void update(float delta);
	virtual void draw();

	virtual void keyDown(ci::app::KeyEvent event);
	virtual void keyUp(ci::app::KeyEvent event);

	void onCollision(Body* body) override;

private:

	enum Direction { LEFT, RIGHT, UP, DOWN, DIGGING, FIGHT };

	int playerIdx;
	std::string animation;

	ci::Vec2f linearVelocity;
	float angularVelocity;
	int playerDirection;
	int lastInput;
	const float PLAYER_SPEED = 50.0f;
	bool leftPressed, rightPressed, upPressed, downPressed;

	std::shared_ptr<AnimatedTexture> m_texture;
	std::shared_ptr<Body> m_body;
};
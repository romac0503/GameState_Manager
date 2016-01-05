#include "StdAfx.h"
#include "Player.h"

#include "cinder\gl\gl.h"
#include "cinder\gl\Texture.h"
#include "cinder\ImageIo.h"
#include "cinder\app\App.h"

#include "StringUtil.h"
#include "InputManager.h"
#include "AnimatedTexture.h"
#include "Body.h"
#include "Treasure.h"

using namespace ci;

Player::Player(const ci::Vec2f& _position, float _rotation, int _playerIdx) :
	GameObject(_position, _rotation),
	linearVelocity(0.0f, 0.0f),
	angularVelocity(0.0f),
	playerIdx(_playerIdx),
	playerDirection(Direction::LEFT)
{
	m_texture = std::shared_ptr<AnimatedTexture>(new AnimatedTexture("dwarve.png", ci::Vec2f(100.0f, 80.0f), 6));

	m_texture->addAnimation("standing_left", 0, 3, false);
	m_texture->addAnimation("standing_right", 0, 3, true);
	m_texture->addAnimation("running_left", 4, 9, false);
	m_texture->addAnimation("running_right", 4, 9, true);
	m_texture->addAnimation("running_up", 4, 9, false);
	m_texture->addAnimation("runing_down", 4, 9, false);
	m_texture->addAnimation("digging", 16, 19, false);
	m_texture->addAnimation("fight_left", 10, 13, false);
	m_texture->addAnimation("fight_right", 10, 13, true);
	m_texture->setAnimation("standing_left");
	lastInput = Direction::LEFT;

	Rectf bounds(position - Vec2f(30.0f, 30.0f), 
		position + Vec2f(30.0f, 30.0f));
	m_body = std::shared_ptr<Body>(new Body(bounds, 1.0f, this));
}

Player::~Player()
{

}

void Player::update(float delta)
{
	InputManager* inputManager = InputManager::getInstance();
	inputManager->update();
	Vec2f steering;
	animation = "standing";
	std::string direction;

	if (inputManager->isKeyDown("left", playerIdx))
	{
		steering.x -= 1.0f;
		playerDirection = Direction::LEFT;
		lastInput = playerDirection;
		animation = "running";
	}

	if (inputManager->isKeyDown("right", playerIdx))
	{
		steering.x += 1.0f;
		playerDirection = Direction::RIGHT;
		lastInput = playerDirection;
		animation = "running";
	}

	if (inputManager->isKeyDown("up", playerIdx))
	{
		steering.y -= 1.0f;
		playerDirection = Direction::UP;
		animation = "running";
	}

	if (inputManager->isKeyDown("down", playerIdx))
	{
		steering.y += 1.0f;
		playerDirection = Direction::DOWN;
		animation = "running";
	}

	if (inputManager->isKeyDown("fight", playerIdx))
	{
		playerDirection = Direction::FIGHT;
		animation = "fight";
	}

	if (inputManager->isKeyDown("digging", playerIdx))
	{
		playerDirection = Direction::DIGGING;
		animation = "digging";
	}

	steering.limit(1.0f);

	if (playerDirection == Direction::LEFT)
	{
		direction = "left";
	}
	if (playerDirection == Direction::RIGHT)
	{
		direction = "right";
	}
	if (playerDirection == Direction::UP || playerDirection == Direction::DOWN || playerDirection == Direction::FIGHT)
	{
		direction = lastInput == 0 ? "left" : "right";
	}

	if (animation == "digging")
	{
		direction = lastInput == 0 ? "left" : "right";
		m_texture->setAnimation(animation);
	}
	else 
	{
		direction = lastInput == 0 ? "left" : "right";
		m_texture->setAnimation(StringUtil::format("%s_%s", animation.c_str(), direction.c_str()));
	}

	m_texture->update(delta);

	m_body->setVelocity(steering * PLAYER_SPEED);
	position = m_body->getPosition();
	linearVelocity = steering * PLAYER_SPEED;
	position += linearVelocity * delta;
	m_body->setPosition(position);
}

void Player::draw()
{
	gl::pushModelView();
	{
		m_texture->draw(position, rotation, playerIdx);
		//gl::drawSolidCircle(Vec2f(0, 0), 5, 5);
	}
	gl::popModelView();
}

void Player::keyDown(ci::app::KeyEvent event)
{
}

void Player::keyUp(ci::app::KeyEvent event)
{
}

void Player::onCollision(Body* collider)
{
	GameObject* object = collider->getGameObject();

	Treasure* treasure = dynamic_cast<Treasure*> (object);

	if (treasure != nullptr)
	{
		treasure->markForDelete();
	}
}
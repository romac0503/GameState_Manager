#pragma once

#include <memory>

class Player;

class Controller
{
public:
	Controller(Player* owner):
		m_owner(owner)
	{}

	virtual ~Controller() {};

	virtual void update(float delta) = 0;

protected:
	Player* m_owner;
};
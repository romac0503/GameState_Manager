#pragma once

#include "Controller.h"

class GameObject;

class AiController :
	public Controller
{
public:
	AiController(Player* owner) :
		Controller(owner)
	{}

	void update(float delta) override;

private:
	std::weak_ptr<GameObject> m_target;
};
#pragma once

#include "Controller.h"

class HumanController:
	public Controller
{
public:
	HumanController(Player* owner) :
		Controller(owner)
	{}

	void update(float delta) override;
};
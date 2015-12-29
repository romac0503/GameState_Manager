#include "StdAfx.h"
#include "GameObject.h"

GameObject::GameObject(const ci::Vec2f& _position, float _rotation) :
	position(_position),
	rotation(_rotation),
	m_wantToDie(false),
	m_id(-1)
{}

GameObject::~GameObject()
{}
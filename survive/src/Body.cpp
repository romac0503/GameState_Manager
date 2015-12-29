#include "StdAfx.h"
#include "Body.h"

#include "cinder/CinderMath.h"
#include "Physics.h"
#include <algorithm>

Body::Body(const ci::Rectf& boundingBox, float mass, GameObject* gameObject) :
	m_mass(mass),
	m_invMass(1.0f),
	m_boundingBox(boundingBox),
	m_gameObject(gameObject)
{
	setMass(m_mass);
	Physics::getInstance()->addBody(this);
}

Body::~Body()
{
	Physics::getInstance()->removeBody(this);
}

void Body::setMass(float mass)
{
	m_mass = mass;

	if (m_mass == 0)
	{
		m_invMass = 0;
	}
	else
	{
		m_invMass = 1.0f / m_mass;
	}
}

void Body::draw()
{
	ci::gl::drawStrokedRect(m_boundingBox);
	ci::gl::color(ci::Color(255, 255, 255));
}


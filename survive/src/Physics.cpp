#include "StdAfx.h"
#include "Physics.h"
#include "Body.h"
#include "InputManager.h"
#include "GameObject.h"

using namespace ci;

Physics::Physics():
	m_debugDraw(false),
	m_resolveCollision(true),
	m_positionalCorrection(true)
{
	m_accumulator = 0.0f;
}

void Physics::addBody(Body* body)
{
	m_bodies.push_back(body);
}

void Physics::removeBody(Body* body)
{
	m_bodies.erase(std::find(m_bodies.begin(), m_bodies.end(), body));
}


bool Physics::AABBvsAABB(const ci::Rectf& a, const ci::Rectf& b, ci::Vec2f& normal, float& penetration)
{
	ci::Vec2f n = b.getCenter() - a.getCenter();			// Vector from A to B
	float a_extent = (a.getX2() - a.getX1()) / 2;
	float b_extent = (b.getX2() - b.getX1()) / 2;
	float x_overlap = a_extent + b_extent - abs(n.x);		// Calculate overlap on x axis

	// SAT test on x axis
	if (x_overlap > 0) {
		float a_extent = (a.getY2() - a.getY1()) / 2;		// Calculate half extents along y axis
		float b_extent = (b.getY2() - b.getY1()) / 2;
		float y_overlap = a_extent + b_extent - abs(n.y);	// Calculate overlap on y axis

		// SAT test on y axis
		if (y_overlap > 0) {
			// Find out which axis is axis of least penetration
			if (x_overlap < y_overlap) {
				// Point towards B knowing that n points from A to B
				if (n.x < 0)
					normal = ci::Vec2f(1, 0);
				else
					normal = ci::Vec2f(-1, 0);
				penetration = x_overlap;
				return true;
			}
			else {
				// Point toward B knowing that n points from A to B
				if (n.y < 0)
					normal = ci::Vec2f(0, 1);
				else
					normal = ci::Vec2f(0, -1);
				penetration = y_overlap;
				return true;
			}
		}
	}
	return false;
}

void Physics::resolveCollision(Body* a, Body* b, ci::Vec2f normal)
{
	// Calculate relative velocity
	ci::Vec2f rv = b->getVelocity() - a->getVelocity();

	// Calculate relative velocity in terms of the normal direction
	float velAlongNormal = rv.dot(normal);

	// Do not resolve if velocities are separating
	if (velAlongNormal < 0)
		return;

	// Apply impulse
	ci::Vec2f impulse = velAlongNormal * normal;

	a->setVelocity(a->getVelocity() + 0.5f * impulse);
	b->setVelocity(b->getVelocity() - 0.5f * impulse);
}


void Physics::positionalCorrection(Body* a, Body* b, float penetration, const ci::Vec2f& normal)
{
	const float percent = 0.2f; // usually 20% to 80%
	const float slop = 0.01f; // usually 0.01 to 0.1
	ci::Vec2f correction = std::max(penetration - slop, 0.0f) /
		(a->getInvMass() + b->getInvMass()) * percent * normal;
	a->setPosition(a->getPosition() + a->getInvMass() * correction);
	b->setPosition(b->getPosition() - b->getInvMass() * correction);
}


void Physics::findCollisions()
{
	for (int i = 0; i < (int)m_bodies.size(); i++)
	{
		Body* shape1 = m_bodies[i];

		for (int j = i + 1; j < (int)m_bodies.size(); j++)
		{
			Body* shape2 = m_bodies[j];

			if (shape1 == shape2 || (shape1->getMass() == 0 && shape2->getMass() == 0))
			{
				// if both object don't have a mass or body is the same skip
				continue;
			}

			Vec2f normal;
			float penetration;

			if (AABBvsAABB(shape1->getBoundingBox(), shape2->getBoundingBox(),
				normal, penetration))
			{
				Manifold manifold;
				manifold.body1 = shape1;
				manifold.body2 = shape2;
				manifold.normal = normal;
				manifold.penetration = penetration;

				m_manifolds.push_back(manifold);
			}

			// TODO create manifold
			
		}
	}
}

void Physics::processCollisions()
{
	for (Manifold manifold : m_manifolds)
	{
		ci::Vec2f pos1 = manifold.body1->getBoundingBox().getCenter();
		ci::Vec2f pos2 = manifold.body2->getBoundingBox().getCenter();

		if (m_resolveCollision)
		{
			resolveCollision(manifold.body1, manifold.body2, manifold.normal);
		}

		if (m_positionalCorrection)
		{
			positionalCorrection(manifold.body1, manifold.body2, manifold.penetration, manifold.normal);
		}

		GameObject* gameObject1 = manifold.body1->getGameObject();
		GameObject* gameObject2 = manifold.body2->getGameObject();

		if (gameObject1 != nullptr)
		{
			gameObject1->onCollision(manifold.body2);
		}

		if (gameObject2 != nullptr)
		{
			gameObject2->onCollision(manifold.body1);
		}
	}
}

void Physics::update(float delta)
{
	// define our physics update rate
	const float step = 1.0f / 100.0f;

	m_accumulator += delta;

	while (m_accumulator > step)
	{
		// update physics with a fixed step
		updateStep(step);
		m_accumulator -= step;
	}
}

void Physics::updateStep(float delta)
{
	InputManager* input = InputManager::getInstance();

	if (input->isKeyReleased(ci::app::KeyEvent::KEY_7))
	{
		m_debugDraw = !m_debugDraw;
	}

	if (input->isKeyReleased(ci::app::KeyEvent::KEY_8))
	{
		m_resolveCollision = !m_resolveCollision;
	}

	if (input->isKeyReleased(ci::app::KeyEvent::KEY_9))
	{
		m_positionalCorrection = !m_positionalCorrection;
	}


	// first reset manifolds
	m_manifolds.clear();

	// find collision and create manifold
	findCollisions();

	// now process collisions
	processCollisions();

	// integrate
	for (Body* shape : m_bodies)
	{
		// apply velocity to bodies
		shape->setPosition(shape->getPosition() + shape->getVelocity() * delta);
	}
}

void Physics::draw()
{
	if (m_debugDraw)
	{
		//cout << "debug Draw acitve" << endl;
		for (Manifold manifold : m_manifolds)
		{
			Body& body1 = *manifold.body1;
			Body& body2 = *manifold.body2;

			gl::lineWidth(1.0f);
			ci::gl::color(ci::Color(200, 0, 0));
			ci::gl::drawLine(body1.getPosition(), body2.getPosition());

			gl::lineWidth(2.0f);
			ci::gl::color(ci::Color(255, 0, 0));
			ci::gl::drawLine(body1.getPosition(), body1.getPosition() - manifold.normal * manifold.penetration);
			ci::gl::drawLine(body2.getPosition(), body2.getPosition() + manifold.normal * manifold.penetration);
			gl::lineWidth(1.0f);

			ci::gl::color(ci::Color(255, 255, 255));
		}

		for (Body* body : m_bodies)
		{
			body->draw();
		}
	}
	else
	{
		//cout << "debug Draw not active" << endl;
	}
}

Physics* Physics::getInstance()
{
	static Physics instance;
	return &instance;
}
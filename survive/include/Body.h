#pragma once

#include "Circle.h"

class GameObject;

class Body
{
public:
	/// Constructs a new physical body
	/// \boundingBox the bounds of the body with the position in the center of the rectangle.
	/// \mass the mass of the body. 0 for static objects which shouldn't be moved.
	/// \gameObject a blank pointer to the GameObject.
	Body(const ci::Rectf& boundingBox, const Circle& collRadius, float mass, GameObject* gameObject = nullptr);
	~Body();

	void setBoundingBox(const ci::Rectf& boundingBox)
	{
		m_boundingBox = boundingBox;
	}

	void setCollisionRadius(const Circle& radius)
	{
		m_circle = radius;
	}

	const ci::Rectf& getBoundingBox() const
	{
		return m_boundingBox;
	}

	const Circle& getCollRadius() const
	{
		return m_circle;
	}

	void setPosition(const ci::Vec2f& position)
	{
		m_boundingBox.offsetCenterTo(position);
		m_circle.position = position;
	}

	ci::Vec2f getPosition() const
	{
		return m_boundingBox.getCenter();
	}

	void setVelocity(const ci::Vec2f& velocity)
	{
		m_velocity = velocity;
	}

	ci::Vec2f getVelocity() const
	{
		return m_velocity;
	}

	void setMass(float mass);

	float getMass() const
	{
		return m_mass;
	}

	float getInvMass() const
	{
		return m_invMass;
	}

	void draw();

	void drawBoundingBox();

	void drawCircle();

	GameObject* getGameObject()
	{
		return m_gameObject;
	}

private:
	/// boundingbox of the body
	ci::Rectf m_boundingBox;

	bool m_drawCircle;

	/// collision radius of the body
	Circle m_circle;

	/// velocity of the body
	ci::Vec2f m_velocity;

	/// mass of the body
	float m_mass;

	/// 1.0f / mass or 0.0f if object is static
	float m_invMass;

	/// plain pointer back to the game object
	GameObject* m_gameObject;
};


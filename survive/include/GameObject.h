#pragma once

#include "cinder/Vector.h"

class Body;

class GameObject
{
public:
	GameObject(const ci::Vec2f& _position, float _rotation = 0);
	virtual ~GameObject();

	virtual void update(float delta) {};
	virtual void draw() {};

	// Sets the position of the game object
	void setPosition(const ci::Vec2f& _position)
	{
		position = _position;
	}

	/// Returns the position of the game object
	const ci::Vec2f& getPosition() const
	{
		return position;
	}

	/// Set the rotation of the game object in degree
	void setRotation(float _rotation)
	{
		rotation = _rotation;
	}

	/// Returns the rotation of the game object in degree
	float getRotation() const
	{
		return rotation;
	}

	virtual void onCollision(Body* collider)
	{}

	bool isMarkedForDelete()
	{
		return m_wantToDie;
	}

	void markForDelete()
	{
		m_wantToDie = true;
	}

	int getId()
	{
		return m_id;
	}

	void setId(int id)
	{
		m_id = id;
	}
protected:
	ci::Vec2f position;
	float rotation;
	bool m_wantToDie;
	int m_id;
};
#pragma once

#include "Circle.h"
#include "GlobalVariables.h"

class Body;

/// Singleton class handling a pyhsical simulation
class Physics
{
public:
	struct Manifold
	{
		ci::Vec2f normal;
		float penetration;
		Body* body1;
		Body* body2;
	};

	/// adds a new physical body to the world
	void addBody(Body* body);

	/// removes a physical body from the world
	void removeBody(Body* body);

	void update(float delta);
	void draw();

	void updateStep(float delta);

	static Physics* getInstance();

private:
	Physics();
	~Physics() {}

	Physics(const Physics& p);
	Physics& operator=(Physics const&);

	/// returns true if the 2 rectangles intersect
	/// \normal filled with the collision normal
	/// \penetration set with the collision penetration depth of the two bodies along the collision normal.
	bool AABBvsAABB(const ci::Rectf& a, const ci::Rectf& b, ci::Vec2f& normal, float& penetration);

	bool circleVsCircle(const Circle& a, const Circle& b, ci::Vec2f& normal, float& penetration);

	/// Finds all collision and stores collision information to the manifold list.
	void findCollisions();

	/// Processes all collision in the manifold list
	void processCollisions();

	/// Resolves a collision of 2 bodies by adding an impulse to both objects
	void resolveCollision(Body* a, Body* b, ci::Vec2f normal);

	/// Linear positional corrections of overlapping bodies.
	void positionalCorrection(Body* a, Body* b, float penetrationDepth, const ci::Vec2f& normal);

	/// Draws the positional correction after collision
	void drawPosCorrection(Body& body1, Body& body2);

	/// Draws the Vec2 when Player colliding
	void drawColResolving(Body& body1, Body& body2);
	
	/// a list of all bodies in the world
	std::vector<Body*> m_bodies;

	/// a list of all collisions in this update step.
	std::vector<Manifold> m_manifolds;

	// Accumulator for a more accurate physics calculation
	float m_accumulator;

	bool m_debugDraw;
	bool m_resolveCollision;
	bool m_positionalCorrection;
	bool m_circleCollision;

	// Vec2f for collision resolving
	/*Vec2f m_collResolving1;
	Vec2f m_collResolving2;*/

	// Vec2f for positional correction
	/*Vec2f m_posCorrection1;
	Vec2f m_posCorrection2;*/
};
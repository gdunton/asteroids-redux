//-----------------------------------------------------------
// File: MathTypes.cpp
//-----------------------------------------------------------

#include "MathTypes.h"

#include "../Content/ModelManager.h"

//----------------------------------------------------------------------
// Vector function definitions
//----------------------------------------------------------------------
Vector2 RotatePoint(const Vector2& vec, float angle)
{
	// Rotate the vec around the origin by the angle
	angle = -angle;

	const float s = sin(angle);
	const float c = cos(angle);

	return { (vec.x * c) - (vec.y * s), (vec.x * s) + (vec.y * c) };
}

float distanceBetween(const Vector2& point1, const Vector2& point2)
{
	Vector2 from = point1 - point2;
	return from.Length();
}

float distanceSquared(const Vector2& point1, const Vector2& point2)
{
	const float dx = point2.x - point1.x;
	const float dy = point2.y - point1.y;

	return dx * dx + dy * dy;
}

float LengthSquared(const Vector2& vector)
{
	return (vector.x * vector.x) + (vector.y * vector.y);
}

void Normalize(Vector2& vec)
{
	vec /= vec.Length();
}

float Dot(const Vector2& point1, const Vector2& point2)
{
	return point1.Dot(point2);
}

//-------------------------------------------------------
// World wrap math functions
//-------------------------------------------------------
void WrapVector2(float worldWidth, float worldHeight, Vector2& pos)
{
	// Force the player to wrap around the world
	// Check that the position doesn't wrap around the screen
	if(pos.x < (-worldWidth / 2) || pos.x > (worldWidth / 2))
	{
		pos.x += worldWidth / 2;
		pos.x = NegativeMod(pos.x, static_cast<int>(worldWidth));
		pos.x -= worldWidth / 2;
	}

	if(pos.y < (-worldHeight / 2) || pos.y > (worldHeight / 2))
	{
		pos.y += worldHeight / 2;
		pos.y = NegativeMod(pos.y, static_cast<int>(worldHeight));
		pos.y -= worldHeight / 2;
	}
}

void GetShortestWrappedDistance(const Vector2& point1, const Vector2& point2,
                                float worldWidth, float worldHeight, Vector2& outDistance)
{
	// Get the half distances
	const float hWH = worldHeight / 2;
	const float hWW = worldWidth / 2;

	// Might need to strip off the floating point value?????
	outDistance.x = fmod((point2.x + hWW - point1.x + worldWidth), worldWidth) - hWW;
	outDistance.y = fmod((point2.y + hWH - point1.y + worldHeight), worldHeight) - hWH;
}

//----------------------------------------------------------------------
// Shape functions
//----------------------------------------------------------------------
namespace MathTypes
{
	bool Rectangle::Intersects(const Circle& circle) const
	{
		// Check that objects are seperable
		return !(position.x > (circle.position.x + circle.radius) || 
			(position.x + size.x) < (circle.position.x - circle.radius) || 
			position.y > (circle.position.y + circle.radius) || 
			(position.y + size.y) < (circle.position.y - circle.radius));
	}

	bool Rectangle::Contains(const Circle& circle) const
	{
		// is circle within a rectangle shrunk by the radius of the circle
		const Vector2 pos = circle.position - position;
		return (pos.x - circle.radius) >= 0 && 
			(pos.x + circle.radius) <= size.x && 
			(pos.y - circle.radius) >= 0 && 
			(pos.y + circle.radius) <= size.y;
	}

	void Rectangle::Draw(const Camera& camera, const Model2D& quadModel) const
	{
		World world;
		world.pos = position + static_cast<Vector2>(size / 2);
		world.rot = 0;
		world.scale = size / 2.0f;
		quadModel.Render(world, camera);
	}


	bool Circle::Intersects(const Rectangle& rect) const
	{
		// if rectangle and circle are seperable
		return !(position.x > (rect.position.x + rect.size.x) || 
			(position.x + radius) < (rect.position.x - rect.size.x) ||
			position.y > (rect.position.y + rect.size.y) || 
			(position.y + radius) < (rect.position.y - rect.size.y));
	}
}

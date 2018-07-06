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

	float s = sin(angle);
	float c = cos(angle);

	Vector2 final(0, 0);
	final.x = ((vec.x * c) - (vec.y * s));
	final.y = ((vec.x * s) + (vec.y * c));

	return final;
}

float distanceBetween(const Vector2& point1, const Vector2& point2)
{
	Vector2 from = point1 - point2;
	return from.Length();
}

float distanceSquared(const Vector2& point1, const Vector2& point2)
{
	float dx = point2.x - point1.x;
	float dy = point2.y - point1.y;

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
void WrapVector2(float width, float height, Vector2& pos)
{
	// Force the player to wrap around the world
	// Check that the position doesn't wrap around the screen
	if(pos.x < (-width / 2) || pos.x > (width / 2))
	{
		pos.x += width / 2;
		pos.x = NegativeMod(pos.x, static_cast<int>(width));
		pos.x -= width / 2;
	}

	if(pos.y < (-height / 2) || pos.y > (height / 2))
	{
		pos.y += height / 2;
		pos.y = NegativeMod(pos.y, static_cast<int>(height));
		pos.y -= height / 2;
	}
}

void GetShortestWrappedDistance(const Vector2& p1, const Vector2& p2,
                                float worldW, float worldH, Vector2& out)
{
	// Get the half distances
	const float hWH = worldH / 2;
	const float hWW = worldW / 2;

	// Might need to strip off the floating point value?????
	out.x = fmod((p2.x + hWW - p1.x + worldW), worldW) - hWW;
	out.y = fmod((p2.y + hWH - p1.y + worldH), worldH) - hWH;
}

//----------------------------------------------------------------------
// Shape functions
//----------------------------------------------------------------------
namespace MathTypes
{
	bool Rectangle::Intersects(const Circle& circle) const
	{
		// Check that objects are seperable
		if(position.x > (circle.position.x + circle.radius) ||
			(position.x + size.x) < (circle.position.x - circle.radius) ||
			position.y > (circle.position.y + circle.radius) ||
			(position.y + size.y) < (circle.position.y - circle.radius))
		{
			return false;
		}
		return true;
	}

	bool Rectangle::Contains(const Circle& circle) const
	{
		// is circle within a rectangle shrunk by the radius of the circle
		Vector2 pos = circle.position - position;
		if((pos.x - circle.radius) >= 0 && (pos.x + circle.radius) <= size.x &&
			(pos.y - circle.radius) >= 0 && (pos.y + circle.radius) <= size.y)
		{
			return true;
		}
		return false;
	}

	void Rectangle::Draw(Camera& camera, const Model2D& quadModel) const
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
		if(position.x > (rect.position.x + rect.size.x) ||
			(position.x + radius) < (rect.position.x - rect.size.x) ||
			position.y > (rect.position.y + rect.size.y) ||
			(position.y + radius) < (rect.position.y - rect.size.y))
		{
			return false;
		}
		return true;
	}
}

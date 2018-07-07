//-----------------------------------------------------------
// File: MathTypes.h
// Description: 
//-----------------------------------------------------------

#ifndef MATH_TYPES_H
#define MATH_TYPES_H

#include <utility>
#include "Vector2.h"

class Model2D;
// Forward decleration of external class
class Camera;

constexpr float PI = 3.14159265359f;

//----------------------------------------------
// Vector2 functions
//----------------------------------------------
Vector2 RotatePoint(const Vector2& vec, float angle);

float distanceBetween(const Vector2& point1, const Vector2& point2);
float distanceSquared(const Vector2& point1, const Vector2& point2);
float LengthSquared(const Vector2& vector);

void Normalize(Vector2& vec);

float Dot(const Vector2& point1, const Vector2& point2);


//-------------------------------------------------------
// World wrap math functions
//-------------------------------------------------------
// Math function for wrapping a position around the world
void WrapVector2(float worldWidth, float worldHeight, Vector2& pos);

// Wrap around function that gets the distance between two points based on the size of the world
void GetShortestWrappedDistance(const Vector2& point1, const Vector2& point2,
                                float worldWidth, float worldHeight, Vector2& outDistance);

//-------------------------------------------------------
// Inline functions
//-------------------------------------------------------
inline void Vec2Multiply(const Vector2& vec1, const Vector2& vec2, Vector2& out)
{
	out.x = vec1.x * vec2.x;
	out.y = vec1.y * vec2.y;
}

inline float Clamp(float min, float max, float value)
{
	if(value > max) return max;
	if(value < min) return min;
	return value;
}

inline int Clamp(int min, int max, int value)
{
	if(value > max) return max;
	if(value < min) return min;
	return value;
}

// Returns projected line projected onto baseLine
inline float scalerProject(const Vector2& baseLine, const Vector2& projected)
{
	return Dot(projected, baseLine) / baseLine.Length();
}

// Allow modulas that operates with negative numbers
inline int NegativeMod(int i, int modulas)
{
	return (i % modulas + modulas) % modulas;
}

// Allow modulas that operates with floating point negative numbers
inline float NegativeMod(float i, int modulas)
{
	return static_cast<float>(fmod((fmod(i, modulas) + modulas), modulas));
}

//-----------------------------------------------------
// Structures
//-----------------------------------------------------

// Struct for world data for translation and rendering
struct World
{
	Vector2 pos;
	Vector2 scale;
	float rot;

	World() noexcept : pos(0, 0), scale{ 1, 1 }, rot(0) { }

	World(Vector2 _pos, Vector2 _scale, const float _rot) noexcept :
		pos(std::move(_pos)), scale(std::move(_scale)), rot(_rot)
	{ }

	// transform object position by the world
	Vector2 TransformPoint(const Vector2& p) const
	{
		Vector2 final;
		Vec2Multiply(p, scale, final);
		final = RotatePoint(final, rot);
		final += pos;

		return final;
	}
};

// Namespace for Rectangle and Circle to prevent conflicts
namespace MathTypes
{
	class Rectangle;
	class Circle;

	// Four number rectangle for collision detection
	class Rectangle
	{
	public:
		// Pos represents the far bottom left of a model (Depending on whether in camera or world space)
		Vector2 position;
		Vector2 size;

		Rectangle(Vector2 pos, Vector2 size) : 
			position(std::move(pos)), size(std::move(size)) 
		{ }
		Rectangle() noexcept = default;

		bool Intersects(const Rectangle& rect2) const
		{
			// if second box is seperable from box
			return !(rect2.position.x > (position.x + size.x) || 
				(rect2.position.x + size.x) < position.x || 
				rect2.position.y > (position.y + size.y) || 
				(rect2.position.y + rect2.size.y) < position.y);
		}

		void Draw(const Camera& camera, const Model2D& quadModel) const;

		bool Intersects(const Circle& circle) const;
		bool Contains(const Circle& circle) const;
	};

	// Second collision shape
	class Circle
	{
	public:
		Vector2 position;
		float radius = 0.0f;

		Circle() = default;
		Circle(Vector2 pos, const float radius) : 
			position(std::move(pos)), radius(radius) 
		{ }

		bool Intersects(const Circle& circle) const
		{
			// if distance between is less than the two circle radiui then circles intersect
			return distanceSquared(position, circle.position) <= ((radius + circle.radius) * (radius + circle.radius));
		}

		bool Intersects(const Rectangle& rect) const;
	};
}
#endif

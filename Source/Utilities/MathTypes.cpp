//-----------------------------------------------------------
// File: MathTypes.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "MathTypes.h"
#include <math.h>

#include "..\Content\ModelManager.h"

using MathTypes::Circle;
using MathTypes::Rectangle;

//----------------------------------------------------------------------
// Vector function definitions
//----------------------------------------------------------------------
MyVector2 RotatePoint( const MyVector2& vec, float angle )
{
	// Rotate the vec around the origin by the angle
	angle = -angle;

	float s = sin(angle);
	float c = cos(angle);

	MyVector2 final(0, 0);
	final.x = ((vec.x * c) - (vec.y * s));
	final.y = ((vec.x * s) + (vec.y * c));

	return final;
}

float distanceBetween( const MyVector2& point1, const MyVector2& point2 )
{
	MyVector2 from = point1 - point2;
	return from.Length();
}

float distanceSquared( const MyVector2& p1, const MyVector2& p2 )
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	return dx*dx + dy*dy;
}

float Length( const MyVector2& vector )
{
	return vector.Length();
}

float LengthSquared( const MyVector2& vector )
{
	return (vector.x * vector.x) + (vector.y * vector.y);
}

void Normalize( MyVector2& vec )
{
	float length = vec.Length();
	vec /= length;
}

float Dot( const MyVector2& point1, const MyVector2& point2 )
{
	return point1.Dot(point2);
}

//-------------------------------------------------------
// World wrap math functions
//-------------------------------------------------------
void WrapVector2( int width, int height, MyVector2& pos )
{
	// Force the player to wrap around the world
	// Check that the position doesn't wrap around the screen
	if( pos.x < (-width / 2) || pos.x > (width / 2) )
	{
		pos.x += width / 2;
		pos.x = NegativeMod( pos.x, width );
		pos.x -= width / 2;
	}

	if( pos.y < (-height / 2) || pos.y > (height / 2))
	{
		pos.y += height / 2;
		pos.y = NegativeMod( pos.y, height );
		pos.y -= height / 2;
	}	
}

void GetShortestWrappedDistance( const MyVector2& p1, const MyVector2& p2, 
	const int worldW, const int worldH, MyVector2& out )
{
	// Get the half distances
	double hWH = worldH / 2;
	double hWW = worldW / 2;

	// Might need to strip off the floating point value?????
	out.x = fmod((p2.x + hWW - p1.x + worldW) , worldW ) - hWW;
	out.y = fmod((p2.y + hWH - p1.y + worldH) , worldH ) - hWH;
}

//----------------------------------------------------------------------
// Shape functions
//----------------------------------------------------------------------

bool MathTypes::Rectangle::Intersects( const Circle& circle ) const
{
	// Check that objects are seperable
	if( position.x > (circle.position.x + circle.radius) ||
		(position.x + size.x) < (circle.position.x - circle.radius) ||
		position.y > (circle.position.y + circle.radius) ||
		(position.y + size.y) < (circle.position.y - circle.radius) )
	{
		return false;
	}
	return true;
}

bool MathTypes::Rectangle::Contains( const Circle& circle ) const
{
	// is circle within a rectangle shrunk by the radius of the circle
	MyVector2 pos = circle.position - position;
	if( (pos.x - circle.radius) >= 0 && (pos.x + circle.radius) <= size.x &&
		(pos.y - circle.radius) >= 0 && (pos.y + circle.radius) <= size.y )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MathTypes::Rectangle::Draw( Camera& camera )
{
	World world;
	world.pos = position + static_cast<MyVector2>(size / 2);
	world.rot = 0;
	world.scale = size / 2.0f;
	ModelManager::GetModel("Quad")->Render(camera, world);
}

bool MathTypes::Circle::Intersects( const MathTypes::Rectangle& rect ) const
{
	// if rectangle and circle are seperable
	if( position.x > (rect.position.x + rect.size.x) ||
		(position.x + radius) < (rect.position.x - rect.size.x) ||
		position.y > (rect.position.y + rect.size.y) ||
		(position.y + radius) < (rect.position.y - rect.size.y) )
	{
		return false;
	}
	return true;
}
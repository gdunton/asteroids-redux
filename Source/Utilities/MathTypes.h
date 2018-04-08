//-----------------------------------------------------------
// File: MathTypes.h
// Description: 
//-----------------------------------------------------------

#ifndef MATH_TYPES_H
#define MATH_TYPES_H

// Forward decleration of external class
class Camera;

//----------------------------------------------
// MyVector2 functions
//----------------------------------------------
MyVector2 RotatePoint( const MyVector2& vec, float angle );

float distanceBetween( const MyVector2& point1, const MyVector2& point2 );
float distanceSquared( const MyVector2& point1, const MyVector2& point2 );
float Length( const MyVector2& vector );
float LengthSquared( const MyVector2& vector );

void Normalize( MyVector2& vec );

float Dot( const MyVector2& point1, const MyVector2& point2 );


//-------------------------------------------------------
// World wrap math functions
//-------------------------------------------------------
// Math function for wrapping a position around the world
void WrapVector2( int worldWidth, int worldHeight, MyVector2& pos );

// Wrap around function that gets the distance between two points based on the size of the world
void GetShortestWrappedDistance( const MyVector2& point1, const MyVector2& point2, 
	const int worldWidth, const int worldHeight, MyVector2& outDistance );

//-------------------------------------------------------
// Inline functions
//-------------------------------------------------------
inline void Vec2Multiply( const MyVector2& vec1, const MyVector2& vec2, MyVector2& out )
{
	 out.x = vec1.x * vec2.x;
	 out.y = vec1.y * vec2.y;
}

inline float Clamp( float min, float max, float value )
{
	if( value > max ) return max;
	else if( value < min ) return min;
	else return value;
}

inline int Clamp( int min, int max, int value )
{
	if( value > max ) return max;
	else if( value < min ) return min;
	else return value;
}

// Returns projected line projected onto baseLine
inline float scalerProject( const MyVector2& baseLine, const MyVector2& projected )
{
	return Dot( projected, baseLine ) / Length( baseLine );
}

// Allow modulas that operates with negative numbers
inline int NegativeMod( int i, int modulas )
{
	return ( i % modulas + modulas) % modulas;
}

// Allow modulas that operates with floating point negative numbers
inline float NegativeMod( float i, int modulas )
{
	return fmod((fmod( i, modulas) + modulas), modulas);
}

//-----------------------------------------------------
// Structures
//-----------------------------------------------------

// Struct for world data for translation and rendering
struct World
{
	MyVector2 pos;
	MyVector2 scale;
	float rot;

	World() : pos(0, 0), scale{ 1, 1 }, rot(0) { }
	World( const MyVector2& _pos, const MyVector2& _scale, const float _rot ) : pos( _pos ), scale(_scale), rot( _rot ) { }

	// transform object position by the world
	MyVector2 TransformPoint( const MyVector2& p ) const
	{
		MyVector2 final;
		Vec2Multiply( p, scale, final );
		final = RotatePoint( final, rot );
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
		MyVector2 position;
		MyVector2 size;

		Rectangle( MyVector2 pos, MyVector2 _size ) : position(pos), size(_size) { }
		Rectangle() : position( MyVector2(0, 0) ), size( MyVector2(0, 0) ) { }

		bool Intersects( const Rectangle& rect2 ) const
		{
			// if second box is seperable from box
			if( rect2.position.x > (position.x + size.x) ||
				(rect2.position.x + size.x) < position.x ||
				rect2.position.y > (position.y + size.y) ||
				(rect2.position.y + rect2.size.y) < position.y )
			{
				return false;
			}
			return true;
		}
		bool Intersects( const Circle& circle ) const;
		bool Contains( const Circle& circle ) const;

		void Draw( Camera& camera );
	};

	// Second collision shape
	class Circle
	{
	public:
		MyVector2 position;
		float radius;

		Circle( const MyVector2& pos, const float _radius ) : position( pos ), radius( _radius ) { }
		Circle() : position( MyVector2(0, 0) ), radius( 0.0f ) { }

		bool Intersects( const Circle& circle ) const
		{
			// if distance between is less than the two circle radiui then circles intersect
			if( distanceSquared( position, circle.position ) <= ((radius + circle.radius)*(radius + circle.radius)) )
			{
				return true;
			}
			return false;
		}
		bool Intersects( const Rectangle& rect ) const;
	};
};
#endif
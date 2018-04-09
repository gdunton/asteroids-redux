//-----------------------------------------------------------
// File: CollisionShape.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "CollisionShape.h"

#include "..\GameMain\Globals.h"

using std::vector;
using std::shared_ptr;

CollisionShape::CollisionShape() : 
	wrapAround( true )
{
}

// empty points and add all the points passed in
void CollisionShape::Initialize( const vector<Vector2>& p )
{
	// Clear all of the previous points
	points.clear();
    
    // Set each point from the list
	points.assign( p.begin(), p.end() );
}

// Set the scale, rotation and position of all the points
void CollisionShape::SetWorld( const World& w )
{
	world = World( w );
}

// Get the transformed points. Check whether they are seperable, return the minimum resolution
// vector so that the objects can be moved.
bool CollisionShape::CheckCollision( CollisionShape& s2, Vector2& outResolution )
{
	// Move both objects so that they are at the center of the world. Stop issued with wrap around world
	Vector2 moveDistance(0, 0);
	//	if there is a difference between the shortest distance and the wrap around distance
	Vector2 wrappedDistance;
	if( wrapAround )
	{
		GetShortestWrappedDistance( s2.world.pos, world.pos, WORLD_WIDTH, WORLD_HEIGHT, wrappedDistance );
	}
	else
	{
		wrappedDistance = s2.world.pos - world.pos; 
	}
	float wrapped = Length( wrappedDistance );
	float distance = distanceBetween( s2.world.pos, world.pos );
	if( abs(wrapped - distance) > 0.1f) // allow for some difference
	{
		// move the objects both to the center of the world by the position of and object
		moveDistance = world.pos;
		world.pos -= moveDistance;
		s2.world.pos -= moveDistance;

		// wrap both positions by the world size
		WrapVector2( WORLD_WIDTH, WORLD_HEIGHT, s2.world.pos );
	}	

	// Get the transformed points
	shared_ptr<vector<Vector2>> ptr1 = GetTransformedPoints();
	shared_ptr<vector<Vector2>> ptr2 = s2.GetTransformedPoints();

	Vector2 mtd1(0,0);
	Vector2 mtd2(0,0);

	// Check whether the two list are seperable. Each one must be checked to get an
    // accurate required movement vector
	if( CollisionShape::CheckPointListSeperability( *ptr1.get(), *ptr2.get(), mtd1 ) )
	{
		// move the objects back
		RestorePosition( moveDistance, world.pos );
		RestorePosition( moveDistance, s2.world.pos );
		return false;
	}
	if( CollisionShape::CheckPointListSeperability( *ptr2.get(), *ptr1.get(), mtd2 ) )
	{
		// move the objects back
		RestorePosition( moveDistance, world.pos );
		RestorePosition( moveDistance, s2.world.pos );
		return false;
	}

	// find the shortest resolution vector
	mtd1 = (LengthSquared( mtd1 ) < LengthSquared(mtd2) ? mtd1 : mtd2 );
	if( LengthSquared( mtd1 ) > 0 )
	{
		// check that the movement vector needs to be reversed
		if( Dot( mtd1, world.pos - s2.world.pos ) < 0.0f )
		{
			mtd1 = -mtd1;
		}
	}
	outResolution = mtd1;

	// move both objects back to their starting positions
	RestorePosition( moveDistance, world.pos );
	RestorePosition( moveDistance, s2.world.pos );

	return true;
}

// Transform each point by the world values and return a smart pointer
// to an allocated vector storing all the points
const shared_ptr<vector<Vector2>> CollisionShape::GetTransformedPoints() const 
{
	shared_ptr<vector<Vector2>> ptr(new vector<Vector2>());

	for( auto begin = points.begin(), end = points.end(); begin != end; ++begin )
	{
		ptr->push_back( world.TransformPoint( *begin ) );
	}

	return ptr;
}

// Check each edge in a list against another set of points to determine whether the
// two shaped intersect
bool CollisionShape::CheckPointListSeperability( vector<Vector2>& p1, vector<Vector2>& p2,
												Vector2& outMtd )
{
	std::vector<Vector2> mtds;

	// For each number if sides
	for( unsigned int i = 0; i < p1.size()-1; i++ )
	{
		//	Create the seperation axis as normal to a side
		Vector2 seperator = p1[i] - p1[(i+1) % (p1.size()-1)];
		seperator = Vector2(seperator.y, -seperator.x);
		Normalize( seperator );

		//	project each of the points on the shapes to find a max and a min along the seperation axis
		// find the max and the min of the first object
		float t1Max, t1Min;
		t1Max = t1Min = scalerProject( seperator, p1[0] );
		for( unsigned int i = 1; i < (p1.size()-1); i++ ) // ignore first value as it has been used to initialize the max and min
		{
			float projI = scalerProject( seperator, p1[i] );
			if( projI > t1Max ) t1Max = projI;
			else if( projI < t1Min ) t1Min = projI;
		}
		float t2Max, t2Min;
		t2Max = t2Min = scalerProject( seperator, p2[0] );
		for( unsigned int i = 1; i < (p2.size()-1); i++ )
		{
			float projI = scalerProject( seperator, p2[i] );
			if( projI > t2Max ) t2Max = projI;
			else if( projI < t2Min ) t2Min = projI;
		}
		
		//  if the max and the min can be seperated then the objects can be seperated so return false
		if( t1Min > t2Max || t2Min > t1Max )
		{
			// Objects are seperable
			return true;
		}
		else
		{
			// Calculate the overlap to work out the minimum translation distance
			Vector2 tempMTD;
			float d0 = t1Max - t2Min;
			float d1 = t2Max - t1Min;
			float depth = (d0 < d1) ? d0 : d1;

			mtds.push_back(seperator * depth);
		}
	}

	outMtd = mtds[0];
	// Get the minimum depth to return
	for( int i = 1; i < mtds.size(); i++ )
	{
		if( LengthSquared(mtds[i]) < LengthSquared(outMtd) )
		{
			outMtd = mtds[i];
		}
	}

	return false;
}

// Checks whether an object has been moved due to world wrap and moves it back into
// position
void CollisionShape::RestorePosition( Vector2& moveDistance, Vector2& outPosition )
{
	if( distanceSquared( moveDistance, Vector2(0,0) ) != 0 ) // then object needs to be moved
	{
		outPosition += moveDistance;
		if( wrapAround )
		{
			WrapVector2( WORLD_WIDTH, WORLD_HEIGHT, outPosition );
		}
	}
}
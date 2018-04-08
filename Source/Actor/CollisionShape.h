//-----------------------------------------------------------
// File: CollisionShape.h
// Description: CollisionShape is used by the PhysicsObject to
//              determine whether objects have collided or not
//-----------------------------------------------------------

#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

class CollisionShape
{
private:
	std::vector<MyVector2> points;
	World world;

    // Whether the object need to wrap around the edge of the world
	bool wrapAround;

public:
	 CollisionShape();

	 void Initialize( const std::vector<MyVector2>& points );

	 // Set the coordinates which creates adjusted
	 void SetWorld( const World& world );

	 // Returns true if a collision occurs and sets a push resolution vector
	 bool CheckCollision( CollisionShape& shape, MyVector2& outResolution );

	 void SetWrapAround( bool value ) { wrapAround = value; }
	  
private:
	 // Get the points transformed by the world data
	 const std::shared_ptr<std::vector<MyVector2>> GetTransformedPoints() const;

	 // Return true if the two arrays of points can be seperated.
	 // Gives back the minimum resolution vector
	 bool CheckPointListSeperability( std::vector<MyVector2>& p1, std::vector<MyVector2>& p2,
				MyVector2& outMtd );

     // If an object has been moved due to world wrap then move it back
	 void RestorePosition( MyVector2& moveDistance, MyVector2& outPosition );
};

#endif
//-----------------------------------------------------------
// File: CollisionShape.h
// Description: CollisionShape is used by the ActorBase to
//              determine whether objects have collided or not
//-----------------------------------------------------------

#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include <memory>
#include <vector>
#include "../Utilities/Vector2.h"
#include "../Utilities/MathTypes.h"

class CollisionShape
{
private:
	std::vector<Vector2> points;
	World world;

	// Whether the object need to wrap around the edge of the world
	bool wrapAround;

public:
	CollisionShape();

	void Initialize(const std::vector<Vector2>& points);

	// Set the coordinates which creates adjusted
	void SetWorld(const World& world);

	// Returns true if a collision occurs and sets a push resolution vector
	bool CheckCollision(CollisionShape& shape, Vector2& outResolution);

	void SetWrapAround(bool value) { wrapAround = value; }

private:
	// Get the points transformed by the world data
	std::shared_ptr<std::vector<Vector2>> GetTransformedPoints() const;

	// Return true if the two arrays of points can be seperated.
	// Gives back the minimum resolution vector
	static bool CheckPointListSeperability(std::vector<Vector2>& p1, std::vector<Vector2>& p2,
	                                Vector2& outMtd);

	// If an object has been moved due to world wrap then move it back
	void RestorePosition(Vector2& moveDistance, Vector2& outPosition);
};

#endif

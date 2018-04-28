//-----------------------------------------------------------
// File: Quadtree.h
// Description: 
//-----------------------------------------------------------

#ifndef QUADTREE_NODE_H
#define QUADTREE_NODE_H
#include "../Graphics/Font.h"

#include "../Utilities/MathTypes.h"
#include <vector>

class Model2D;
class PhysicsObject;

class Quadtree
{
public:
	friend Quadtree;

	Quadtree();

	// Initializer to set the bounding rectangle and the parent
	void Initialize( Quadtree* parent, MathTypes::Rectangle rect, int level );

	// Check all objects are in the right quad. Move if not
	// perform the collision on the objects
	void Update();
	
	// Adds an object into the top quad and attempts to send the object lower in
	// the tree
	bool AddPhysicsObject(const std::shared_ptr<PhysicsObject>& object);

	// Remove the object from it's quad. If a quad recieves a true from a child
	// then it will stop further calculation.
	bool RemovePhysicsObject(const std::shared_ptr<PhysicsObject>& obj);

	// Check an individual physicsobject for collision and compute collision after
	bool ComputeIndividual( PhysicsObject& object );

	// Remove all elements
	void Reset();

	void DrawQuads(Camera& camera, const Model2D& quadModel);

	// Get the number of physics objects in the quad and children
	int NumPhysicsObjects();

private:

	// Check that all objects are in the correct quad.
	// This involves determining if an object is completely enclosed in
	// the quad. if not send it to the parent quad
	void CheckObjects();

	// check all the collisions in the quad. If there are child quads
	// then check that the object overlaps them and pass it to them
	// to check for collisions.
	void CheckCollisions();

	// Checks an object against any children. Specifically for objects
	// that overlap quads. Can also pass onto other children quads.
	void ComputeCollisionAgainstChildren( PhysicsObject& ob );

	// Adds children to the tree and attempts to pass the objects down.
	// contrained by the maximum levels. returns false if max level reached
	bool SplitQuad();

	// Check that the quad and children have less physics objects in them than 
	// the maximum children
	bool Shrinkable();

	// Move all the physics objects from the children into the current quad
	// then remove all of the children.
	void ShrinkQuad();

	// Insert a physics object called from a lower level quad. Used for moving 
	// objects up the tree until they find a quad they fit in
	void InsertFromLower(std::shared_ptr<PhysicsObject> object);

	// Insert from higher add object to lower level quad. Used for attempting 
	// to send object down the tree
	bool InsertFromHigher(std::shared_ptr<PhysicsObject> object);

	// Inserts the object into the actual array
	void AddObject(const std::shared_ptr<PhysicsObject>& ob);
	

	// Vector with all the different quads. Can have either 4 or 0 children
	std::vector<Quadtree> children;

	// Parent quad. Can be one or null
	Quadtree* parent;

	// Container of references to all the physicsObjects
	std::vector<std::shared_ptr<PhysicsObject>> physObjs;

	// Rectangle containing position and size of the quad
	MathTypes::Rectangle bounds;

	// The number of times the quad has been divided. Top quad is level 0
	int level;

	// The maximum number of physics objects that can be in one quad.
	// Not a hard value as it may not be possible to move objects to other quads
	static const int MAX_CHILDREN;
	// The maximum number of levels for quads. Higher priority than MAX_CHILDREN
	// if max levels has been reached then MAX_CHILDREN will be exceeded.
	static const int MAX_DEPTH;


};

#endif
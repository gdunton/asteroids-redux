//-----------------------------------------------------------
// File: Quadtree.cpp
//-----------------------------------------------------------

#include "Quadtree.h"

#include "ActorBase.h"
#include <algorithm>
#include "../Debugging/Error.h"

const int Quadtree::MAX_CHILDREN = 5;
const int Quadtree::MAX_DEPTH = 6;

Quadtree::Quadtree() noexcept
{
	children = std::vector<Quadtree>();
	children.reserve(4);
	parent = nullptr;

	bounds = MathTypes::Rectangle();

	level = -1;
}


// Initializer to set the bounding rectangle and the parent
void Quadtree::Initialize(Quadtree* newParent, MathTypes::Rectangle newRect, int newLevel)
{
	parent = newParent;

	bounds = newRect;
	level = newLevel;
}


// Check all objects are in the right quad. Move if not
// perform the collision on the objects
void Quadtree::Update()
{
	// Check that all objects are in the correct quads
	CheckObjects();

	// compute collisions for all objects
	CheckCollisions();

	// Update all children
	for(Quadtree& i : children)
	{
		i.Update();
	}

	// Attempt to shrink the tree. Must be last operation so that it happens to the
	// lowest level child first
	if(Shrinkable())
	{
		ShrinkQuad();
	}
}


// Adds an object into the top quad and attempts to send the object lower in
// the tree
bool Quadtree::AddPhysicsObject(const std::shared_ptr<ActorBase>& object)
{
	// Make sure that the quad is top level
	ASSERT( level == 0 );

	bool insertionOccured = false;
	for(Quadtree& child : children)
	{
		if(child.InsertFromHigher(object))
		{
			insertionOccured = true;
			break;
		}
	}
	if(!insertionOccured)
	{
		AddObject(object);
		// Check that the quad doesn't need to be split
		if(physObjs.size() > MAX_CHILDREN)
		{
			// split the quads
			SplitQuad();
		}
	}
	return true;
}


// Remove the object from it's quad. If a quad recieves a true from a child
// then it will stop further calculation.
bool Quadtree::RemovePhysicsObject(const std::shared_ptr<ActorBase>& obj)
{
	// Search in current quad for the object
	const auto iter = std::find(physObjs.begin(), physObjs.end(), obj);
	if(iter != physObjs.end())
	{
		physObjs.erase(iter);
		return true;
	}

	// For each child
	for(Quadtree& ch : children)
	{
		// if object is found and removed
		if(ch.RemovePhysicsObject(obj))
		{
			return true;
		}
	}

	return false;
}


// Check an individual physicsobject for collision and compute collision after
bool Quadtree::ComputeIndividual(ActorBase& object)
{
	// Check against each object
	if(bounds.Intersects(object.GetCircle()))
	{
		for(auto& phys : physObjs)
		{
			if(phys->CompleteCollisionCompute(object))
			{
				return true;
			}
		}
		// Then check against each child
		for(Quadtree& child : children)
		{
			if(child.ComputeIndividual(object))
			{
				return true;
			}
		}
	}
	return false;
}


// Check that all objects are in the correct quad.
// This involves determining if an object is completely enclosed in
// the quad. if not send it to the parent quad
void Quadtree::CheckObjects()
{
	for(auto& i : physObjs)
	{
		// Check that the object is still contained in the quad
		if(bounds.Contains(i->GetCircle()))
		{
			// if so then check whether is can be moved into the children
			for(Quadtree& child : children)
			{
				if(child.InsertFromHigher(i))
				{
					// successfully inserted physics object into child quad
					i = nullptr;
					break;
				}
			}
		}
		else
		{
			// else pass to the parent if available
			if(parent)
			{
				parent->InsertFromLower(i);
				i = nullptr;
			}
		}
	}

	physObjs.erase(std::remove(physObjs.begin(), physObjs.end(), nullptr), physObjs.end());
}


// check all the collisions in the quad. If there are child quads
// then check that the object overlaps them and pass it to them
// to check for collisions.
void Quadtree::CheckCollisions()
{
	// Compute the collisions of each object against those stored in the children.
	for(auto begin = physObjs.begin(); begin != physObjs.end(); ++begin)
	{
		for(Quadtree& tree : children)
		{
			tree.ComputeCollisionAgainstChildren(**begin);
		}

		// Then compute the collisions of all the objects against each other
		for(auto b2 = begin + 1; b2 != physObjs.end(); ++b2)
		{
			Vector2 res(0, 0);
			if((*begin)->CheckCollision(**b2, res))
			{
				(*begin)->AdvancedMoveApart(**b2, res);
				(*begin)->PerformCollisionCalculation(**b2);
			}
		}
	}
}


// Checks an object against any children. Specifically for objects
// that overlap quads. Can also pass onto other children quads.
void Quadtree::ComputeCollisionAgainstChildren(ActorBase& ob)
{
	// Check the object against the quad
	if(bounds.Intersects(ob.GetCircle()))
	{
		// then check object against all children objects
		for(auto& phys : physObjs)
		{
			Vector2 res(0, 0);
			if(ob.CheckCollision(*phys, res))
			{
				ob.AdvancedMoveApart(*phys, res);
				ob.PerformCollisionCalculation(*phys);
			}
		}

		// send object to each child quad
		for(Quadtree& tree : children)
		{
			tree.ComputeCollisionAgainstChildren(ob);
		}
	}
}


// Adds children to the tree and attempts to pass the objects down.
// contrained by the maximum levels. returns false if max level reached
bool Quadtree::SplitQuad()
{
	if((level >= MAX_DEPTH) || (!children.empty()))
	{
		return false; // Cannot create more levels
	}

	// Create the new children and add them
	Quadtree newChildren[4];
	// Initialize each of the new children
	newChildren[0].Initialize(this, MathTypes::Rectangle(bounds.position, bounds.size / 2), level + 1);
	newChildren[1].Initialize(this, MathTypes::Rectangle(bounds.position + Vector2(bounds.size.x / 2, 0), bounds.size / 2), level + 1);
	newChildren[2].Initialize(this, MathTypes::Rectangle(bounds.position + Vector2(0, bounds.size.y / 2), bounds.size / 2), level + 1);
	newChildren[3].Initialize(this, MathTypes::Rectangle(bounds.position + static_cast<Vector2>(bounds.size / 2), bounds.size / 2), level + 1);
	children.assign(newChildren, newChildren + 4);

	// Attempt to insert each of the physics objects into the children
	for(auto begin = physObjs.begin(); begin != physObjs.end(); ++begin)
	{
		for(Quadtree& child : children)
		{
			if(child.InsertFromHigher(*begin))
			{
				*begin = nullptr;
				break;
			}
		}
	}

	// Remove all the nullptr objects
	physObjs.erase(std::remove(physObjs.begin(), physObjs.end(), nullptr), physObjs.end());

	return true;
}


// Check that the quad and children have less physics objects in them than 
// the maximum children
bool Quadtree::Shrinkable()
{
	// if Number of physics objects is less than the max children return true
	bool shrink = NumPhysicsObjects() <= MAX_CHILDREN;
	if(children.empty()) 
		shrink = false;
	return shrink;
}


// Move all the physics objects from the children into the current quad
// then remove all of the children.
void Quadtree::ShrinkQuad()
{
	// Move all the phys obs from the children into the current quad
	for(Quadtree& child : children)
	{
		for(auto& ob : child.physObjs)
		{
			AddObject(ob);
		}
	}

	// Remove the children
	children.clear();
}

// Insert a physics object called from a lower level quad. Used for moving 
// objects up the tree until they find a quad they fit in
void Quadtree::InsertFromLower(const std::shared_ptr<ActorBase>& object)
{
	// Top level needs to accept all objects
	if(level <= 0)
	{
		AddObject(object);
		return;
	}

	// object doesn't go in quad send further upwards
	if(!bounds.Contains(object->GetCircle()))
	{
		parent->InsertFromLower(object);
	}
	else // Quad contains object. Attempt to send lower
	{
		bool insertionOccured = false;
		for(Quadtree& child : children)
		{
			if(child.InsertFromHigher(object))
			{
				insertionOccured = true;
				break;
			}
		}
		if(!insertionOccured)
		{
			AddObject(object); // couldn't go to child. Insert in current
		}
	}
}

// Insert from higher add object to lower level quad. Used for attempting 
// to send object down the tree
bool Quadtree::InsertFromHigher(const std::shared_ptr<ActorBase>& object)
{
	if(level <= 0)
	{
		// WTF did this object come from
		AddObject(object);
		return true;
	}

	// object doesn't go in quad so return false
	if(!bounds.Contains(object->GetCircle()))
	{
		return false;
	}
	// Quad contains object. Attempt to send lower
	bool insertionOccured = false;
	for(Quadtree& child : children)
	{
		if(child.InsertFromHigher(object))
		{
			insertionOccured = true;
			break;
		}
	}
	if(!insertionOccured)
	{
		AddObject(object);
		// Check that the quad doesn't need to be split
		if(physObjs.size() > MAX_CHILDREN)
		{
			// split the quads
			SplitQuad();
		}
	}

	return true;
}

// Inserts the object into the actual array
void Quadtree::AddObject(const std::shared_ptr<ActorBase>& object)
{
	physObjs.push_back(object);
}

void Quadtree::Reset()
{
	if(level == 0)
	{
		children.clear();
		physObjs.clear();
	}
}

void Quadtree::DrawQuads(const Camera& camera, const Model2D& quadModel)
{
#ifdef PHYSICS_DEBUG_INFO
	bounds.Draw(camera, quadModel);
	// Draw name of quad
	Vector2 pos = camera.Transform(bounds.position + static_cast<Vector2>(bounds.size / 2));

	// Draw the id of the quad on the position of each asteroid
	for(auto& phys : physObjs)
	{
		Vector2 pPos = camera.Transform(phys->GetPos());
	}

	for(Quadtree& c : children)
	{
		c.DrawQuads(camera, quadModel);
	}
#else
	(void)camera;
	(void)quadModel;
#endif
}

int Quadtree::NumPhysicsObjects()
{
	auto amount = static_cast<int>(physObjs.size());

	// Add the number in each child
	for(Quadtree& child : children)
	{
		amount += child.NumPhysicsObjects();
	}

	return amount;
}

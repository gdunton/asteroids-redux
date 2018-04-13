//-----------------------------------------------------------
// File: Quadtree.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Quadtree.h"

#include "PhysicsObject.h"

#ifdef DEBUG
#include "../Graphics/DebugFont.h"
#endif

const int Quadtree::MAX_CHILDREN = 5;
const int Quadtree:: MAX_DEPTH = 6;

Quadtree::Quadtree()
{
	children = std::vector<Quadtree>();
	children.reserve(4);
	parent = nullptr;

	physObjs = std::list<PhysicsObjectRef>();

	bounds = MathTypes::Rectangle();

	level = -1;
}


// Initializer to set the bounding rectangle and the parent
void Quadtree::Initialize( Quadtree* _parent, MathTypes::Rectangle _rect, int _level )
{
	parent = _parent;

	bounds = _rect;
	level = _level;

#ifdef PHYSICS_DEBUG_INFO
	id = level;
#endif
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
	for (Quadtree& i : children)
	{
		i.Update();
	}

	// Attempt to shrink the tree. Must be last operation so that it happens to the
	// lowest level child first
	if( Shrinkable() )
	{
		ShrinkQuad();
	}
}


// Adds an object into the top quad and attempts to send the object lower in
// the tree
bool Quadtree::AddPhysicsObject( PhysicsObject& object )
{
	// Make sure that the quad is top level
	ASSERT( level == 0 );

	bool insertionOccured = false;
	for( Quadtree& child : children )
	{
		if( child.InsertFromHigher( object ) )
		{
			insertionOccured = true;
			break;
		}
	}
	if( !insertionOccured )
	{
		AddObject( object );
		// Check that the quad doesn't need to be split
		if( physObjs.size() > MAX_CHILDREN )
		{
			// split the quads
			SplitQuad();
		}
	}
	return true;
}


// Remove the object from it's quad. If a quad recieves a true from a child
// then it will stop further calculation.
bool Quadtree::RemovePhysicsObject( int id )
{
	// Search in current quad for the object
	for( auto begin = physObjs.begin(), end = physObjs.end(); begin != end; ++begin )
	{
		if( (*begin).id == id )
		{
			physObjs.erase( begin );
			return true;
		}
	}

	// For each child
	for (Quadtree& ch : children )
	{
		// if object is found and removed
		if( ch.RemovePhysicsObject( id ) )
		{
			return true;
		}
	}

	return false;
}


// Check an individual physicsobject for collision and compute collision after
bool Quadtree::ComputeIndividual( PhysicsObject& ob )
{
	bool collisionOccured = false;
	// Check against each object
	if( bounds.Intersects( ob.GetCircle() ) )
	{
		for( PhysicsObjectRef& phys : physObjs )
		{
			PhysicsObject* p = phys.phys;
			if( p->CompleteCollisionCompute( ob ) )
			{
				return true;
			}			
		}
		// Then check against each child
		for( Quadtree& child : children )
		{
			if( child.ComputeIndividual( ob ) )
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
	std::list<PhysicsObjectRef>::iterator i = physObjs.begin();
	while( i != physObjs.end() )
	{
		bool nothingChanged = true;

		// Check that the object is still contained in the quad
		PhysicsObject *p = i->phys;
		if( bounds.Contains( p->GetCircle()) )
		{
			// if so then check whether is can be moved into the children
			for( Quadtree& child : children )
			{
				if( child.InsertFromHigher( *p ) )
				{
					// successfully inserted physics object into child quad
					physObjs.erase( i++ );
					nothingChanged = false;
					break;
				}
			}
		}
		else
		{
			// else pass to the parent if available
			if( parent )
			{
				parent->InsertFromLower( *p ); 
				physObjs.erase( i++ );
				nothingChanged = false;
			}
		}	

		if( nothingChanged ) i++;
	}	
}


// check all the collisions in the quad. If there are child quads
// then check that the object overlaps them and pass it to them
// to check for collisions.
void Quadtree::CheckCollisions()
{
	// Compute the collisions of each object against those stored in the children.
	for( auto begin = physObjs.begin(), end = physObjs.end(); begin != end; ++begin )
	{
		for( Quadtree& tree : children )
		{
			tree.ComputeCollisionAgainstChildren( *(*begin).phys );
		}

		// Then compute the collisions of all the objects against each other
		auto b2 = begin; // use allocator that already exists
		++b2;
		for( ; b2 != end; ++b2 )
		{
			Vector2 res( 0, 0 );
			if( (*begin).phys->CheckCollision( *(*b2).phys, res ) )
			{
				(*begin).phys->AdvancedMoveApart( *(*b2).phys, res );
				(*begin).phys->PerformCollisionCalculation( *(*b2).phys );
			}
		}
	}
}


// Checks an object against any children. Specifically for objects
// that overlap quads. Can also pass onto other children quads.
void Quadtree::ComputeCollisionAgainstChildren( PhysicsObject& ob )
{
	// Check the object against the quad
	if( bounds.Intersects( ob.GetCircle() ) )
	{
		// then check object against all children objects
		for( PhysicsObjectRef& phys : physObjs )
		{
			Vector2 res( 0, 0 );
			if( ob.CheckCollision( *phys.phys, res ) )
			{
				ob.AdvancedMoveApart( *phys.phys, res );
				ob.PerformCollisionCalculation( *phys.phys );
			}
		}

		// send object to each child quad
		for( Quadtree& tree : children )
		{
			tree.ComputeCollisionAgainstChildren( ob );
		}
	}
}


// Adds children to the tree and attempts to pass the objects down.
// contrained by the maximum levels. returns false if max level reached
bool Quadtree::SplitQuad()
{
	if( (level >= MAX_DEPTH) || (children.size() > 0) ) 
	{
		return false; // Cannot create more levels
	}

	// Create the new children and add them
	Quadtree newChildren[4];
	// Initialize each of the new children
	newChildren[0].Initialize( this, MathTypes::Rectangle( bounds.position, bounds.size / 2 ), level + 1 );
	newChildren[1].Initialize( this, MathTypes::Rectangle( bounds.position + Vector2(bounds.size.x / 2, 0), bounds.size / 2 ), level + 1 );
	newChildren[2].Initialize( this, MathTypes::Rectangle( bounds.position + Vector2(0, bounds.size.y / 2), bounds.size / 2 ), level + 1 );
	newChildren[3].Initialize( this, MathTypes::Rectangle( bounds.position + static_cast<Vector2>(bounds.size / 2), bounds.size / 2 ), level + 1 );
#ifdef PHYSICS_DEBUG_INFO
	newChildren[0].id = (level + 1) * 10 + 1;
	newChildren[1].id = (level + 1) * 10 + 2;
	newChildren[2].id = (level + 1) * 10 + 3;
	newChildren[3].id = (level + 1) * 10 + 4;
#endif
	children.assign( newChildren, newChildren+4 );

	// Attempt to insert each of the physics objects into the children
	//for( auto begin = physObjs.begin(); begin != physObjs.end(); ++begin )
	std::list<PhysicsObjectRef>::iterator i = physObjs.begin();
	while( i != physObjs.end() )
	{
		bool insertionHappend = false;
		for( Quadtree& child : children )
		{
			PhysicsObject *p = i->phys;
			if( child.InsertFromHigher( *p ) )
			{
				insertionHappend = true;
				physObjs.erase( i++ );
				break;
			}
			else
			{
			}
		}
		if( !insertionHappend ) i++;
	}

	return true;
}


// Check that the quad and children have less physics objects in them than 
// the maximum children
bool Quadtree::Shrinkable()
{
	// if Number of physics objects is less than the max children return true
	bool shrink = NumPhysicsObjects() <= MAX_CHILDREN;
	if( children.size() == 0 ) shrink = false;
	return shrink;
}


// Move all the physics objects from the children into the current quad
// then remove all of the children.
void Quadtree::ShrinkQuad()
{
	// Move all the phys obs from the children into the current quad
 	for( Quadtree& child : children )
	{
		for( PhysicsObjectRef& ob : child.physObjs )
		{
			PhysicsObject* p = ob.phys;
			AddObject( *p );
		}
	}

	// Remove the children
	children.clear();
}

// Insert a physics object called from a lower level quad. Used for moving 
// objects up the tree until they find a quad they fit in
void Quadtree::InsertFromLower( PhysicsObject& object )
{
	// Top level needs to accept all objects
	if( level <= 0 )
	{
		AddObject( object );
		return;
	}

	// object doesn't go in quad send further upwards
	if( !bounds.Contains( object.GetCircle() ) )
	{
		parent->InsertFromLower( object );
	}
	else // Quad contains object. Attempt to send lower
	{
		bool insertionOccured = false;
		for( Quadtree& child : children )
		{
			int id = object.GetID(); // store the id for later removal
			if( child.InsertFromHigher( object ) )
			{
				insertionOccured = true;
				break;
			}
		}
		if( !insertionOccured )
		{
			AddObject( object ); // couldn't go to child. Insert in current
		}
	}
}

// Insert from higher add object to lower level quad. Used for attempting 
// to send object down the tree
bool Quadtree::InsertFromHigher( PhysicsObject& object )
{
	if( level <= 0 )
	{
		// WTF did this object come from
		AddObject( object );
		return true;
	}

	// object doesn't go in quad so return false
	if( !bounds.Contains( object.GetCircle() ) )
	{
		return false;
	}
	else // Quad contains object. Attempt to send lower
	{
		bool insertionOccured = false;
		for( Quadtree& child : children )
		{
			int id = object.GetID(); // store the id for later removal
			if( child.InsertFromHigher( object ) )
			{
				insertionOccured = true;
				break;
			}
		}
		if( !insertionOccured )
		{
			AddObject( object );
			// Check that the quad doesn't need to be split
			if( physObjs.size() > MAX_CHILDREN )
			{
				// split the quads
				SplitQuad();
			}
		}
	}
	
	
	return true;
}

// Inserts the object into the actual array
void Quadtree::AddObject( PhysicsObject& object )
{
	PhysicsObjectRef ref;
	ref.id = object.GetID();
	ref.phys = &object;
	physObjs.push_back( ref );
}

void Quadtree::Reset()
{
	if( level == 0 )
	{
		children.clear();
		physObjs.clear();
	}
}

void Quadtree::DrawQuads( Camera& camera )
{
#ifdef PHYSICS_DEBUG_INFO
	bounds.Draw( camera );
	// Draw name of quad
	Vector2 pos = camera.ConvertWorldToScreenPos( bounds.position + (bounds.size / 2) );
	DebugFont::DrawString( std::to_string( id ), pos ); 
	// Draw the id of the quad on the position of each asteroid
	for( PhysicsObjectRef& phys : physObjs )
	{
		PhysicsObject* p = phys.phys;
		Vector2 pPos = camera.ConvertWorldToScreenPos( p->GetPos() );
		DebugFont::DrawString( std::to_string( id ), pPos );
	}
	

	for( Quadtree& c : children )
	{
		c.DrawQuads(camera);
	}
#endif
}

int Quadtree::NumPhysicsObjects()
{
	int amount = physObjs.size();

	// Add the number in each child
	for( Quadtree& child : children )
	{
		amount += child.NumPhysicsObjects();
	}

	return amount;
}
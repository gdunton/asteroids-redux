//-----------------------------------------------------------
// File: GameEntity.h
// Description: GameEntity defines everything needed for an
//				object in the world
//-----------------------------------------------------------

#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "../Graphics/Model2D.h"
#include "../Graphics/Camera.h"

class GameEntity
{
public:
	// Constructors
	GameEntity() = default;

	GameEntity( const Vector2& pos, const Vector2& size, const float rot,
		const Model2D& model ) :
		world(pos, size, rot ), model( model ), wrapAroundWorld( true )
	{
		id = ++lastID;
	}

	virtual ~GameEntity() = default;

	// Overrideable rendering
	virtual void Render( Camera& camera ) const;

	// Wrap the position around the game world
	void WrapPosition();

	// Accessors
	Vector2 GetPos() const { return world.pos; }
	Vector2 GetSize() const { return world.scale; }
	float GetRot() const { return world.rot; }
	const Model2D& GetModel() const { return model; }
	const World& GetWorld() const { return world; }
	int GetID() const { return id; }

	void SetWrapAround( bool value ) { wrapAroundWorld = value; }

protected:
	void ResetID() { id = ++lastID; }

	static int lastID;
	int id = 0;

	World world;

	Model2D model;
	bool wrapAroundWorld = true;
};

#endif
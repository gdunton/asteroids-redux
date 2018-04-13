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

	GameEntity( const Vector2& _pos, const Vector2& _size, const float _rot,
		Model2D* _model ) :
		world(_pos, _size, _rot ), model( _model ), wrapAroundWorld( true )
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
	Model2D* GetModel() const { return model; }
	const World& GetWorld() const { return world; }
	int GetID() const { return id; }

	void SetWrapAround( bool value ) { wrapAroundWorld = value; }

protected:
	void ResetID() { id = ++lastID; }

	static int lastID;
	int id = 0;

	World world;

	Model2D* model = nullptr;
	bool wrapAroundWorld = true;
};

#endif
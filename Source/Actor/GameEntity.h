//-----------------------------------------------------------
// File: GameEntity.h
// Description: GameEntity defines everything needed for an
//				object in the world
//-----------------------------------------------------------

#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <utility>
#include "../Graphics/Model2D.h"
#include "../Graphics/Camera.h"
#include "../Utilities/Vector2.h"
#include "../Utilities/MathTypes.h"

class GameEntity
{
public:
	GameEntity() = default;

	GameEntity( const Vector2& pos, const Vector2& size, const float rot,
	            Model2D model ) :
		world(pos, size, rot ), model(std::move(model))
	{
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

	void SetWrapAround( bool value ) { wrapAroundWorld = value; }

protected:

	World world;

	Model2D model;
	bool wrapAroundWorld = true;
};

#endif
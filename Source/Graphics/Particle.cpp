//-----------------------------------------------------------
// File: Particle.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Particle.h"

#include "Camera.h"
using std::vector;

const float Particle::ScaleRate = 1.01f;

Particle::Particle()
{
	points.reserve(2);
	points.push_back(Vector2(0,0));
	points.push_back(Vector2(0,0));
	rotSpeed = 0;
	lifespan = 0;
	rotation = 0;
	alpha = 0;
	initialLife = 0;
	pos = Vector2(0, 0);
	scale = Vector2( 1, 1 );
}

Particle::~Particle()
{
}

void Particle::Initialize( Vector2 _p1, Vector2 _p2, Vector2 _pos, Vector2 _scale, Vector2 _velocity, 
		float rot, float rotationSpeed, float _lifespan )
{
	points[0] = _p1;
	points[1] = _p2;
	velocity = _velocity;
	rotSpeed = rotationSpeed;
	lifespan = _lifespan;
	initialLife = _lifespan;

	rotation = rot;
	scale = _scale;

	pos = _pos;

	alpha = 255;
}

void Particle::Update( float dt )
{
 	pos += velocity * dt;

	rotation += rotSpeed * dt;
	scale *= ScaleRate;

	lifespan -= dt;

	// Update the alpha to fade out as the particle dies
	if( lifespan > 0 )
	{
		alpha = 255 * (lifespan / initialLife);
	}
	else
	{
		alpha = 0;
	}
}

void Particle::Render( Camera& camera )
{
	// Reset the color of the particle with the alpha
	line.SetColor( ColorRGBA( 255, 255, 255, alpha ) );

	line.Render( &camera, points, World(pos, scale, rotation) );
}

bool Particle::Alive()
{
	return (lifespan > 0);
}
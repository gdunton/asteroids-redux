//-----------------------------------------------------------
// File: Particle.h
// Description: Collections of the particle objects are used
//				by the ParticleSystem class to show objects as
//				they die. Each particle fades out as it dies while 
//				also moving according to it's velocity, rotation 
//				speed and a scale rate
//-----------------------------------------------------------

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Line.h"

class Camera;

class Particle
{
public:

	Particle( Vector2 p1, Vector2 p2, Vector2 pos, Vector2 scale, Vector2 velocity,
		float rotation, float rotationSpeed, float lifespan );

	void Update( float dt );
	void Render(const Camera& camera) const;

	bool Alive() const;

private:
	// The amount that the rotation will change each second
	float rotSpeed = 0;
	// The number of seconds that the particle will survive. counts down to zero
	float lifespan = 0;
	// The amount that the line has rotation in total
	float rotation = 0;

	float alpha = 1;
	float initialLife = 1;

	Vector2 scale;

	Vector2 pos;

	Vector2 velocity;
	// The points in the line
	std::vector<Vector2> points;
	Line line;

	// The amound that each particle will stretch by each frame
	constexpr static float ScaleRate = 1.01f;
};

#endif
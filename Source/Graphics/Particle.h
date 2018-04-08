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
private:
	
	// The amount that the rotation will change each second
	float rotSpeed;
	// The number of seconds that the particle will survive. counts down to zero
	float lifespan;
	// The amount that the line has rotation in total
	float rotation;

	float alpha;
	float initialLife;

	MyVector2 scale;

	MyVector2 pos;
	
	MyVector2 velocity;
	// The points in the line
	std::vector<MyVector2> points;
	Line line;

	// The amound that each particle will stretch by each frame
	static const float ScaleRate;

public:
	Particle();
	~Particle();

	void Initialize( MyVector2 p1, MyVector2 p2, MyVector2 pos, MyVector2 scale, MyVector2 velocity, 
		float rotation, float rotationSpeed, float lifespan );

	void Update( float dt );
	void Render( Camera& camera );

	bool Alive();
};

#endif
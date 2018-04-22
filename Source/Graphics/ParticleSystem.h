//-----------------------------------------------------------
// File: ParticleSystem.h
// Description: ParticleSystem has an array of particles which 
//				is updated per frame and is added to by the
//				gameLogic
//-----------------------------------------------------------

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Particle.h"

class Camera;

class ParticleSystem
{
public:
	ParticleSystem();

	void Reset();

	// Adds new particles to the array
	void AddParticles( std::vector<Particle>& newParticles );

	void Update( float dt );
	void Render( Camera& camera );

private:
	std::vector<Particle> particleArray;

	// The initial size of the particleArray.
	static const int BUFFER_SIZE;
};

#endif
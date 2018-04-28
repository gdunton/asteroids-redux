//-----------------------------------------------------------
// File: ParticleSystem.cpp
//-----------------------------------------------------------

#include "ParticleSystem.h"

const int ParticleSystem::BUFFER_SIZE = 20;

ParticleSystem::ParticleSystem()
{
	// reserve memory for the array to reduce excessive reallocation in the early game
	particleArray.reserve( BUFFER_SIZE );
}

void ParticleSystem::Reset()
{
	particleArray.clear();
}

void ParticleSystem::AddParticles( std::vector<Particle>& particles )
{
	// Index for the objects in "particles"
	size_t preAllocated = 0;

	// Attempt to allocate objects into existing space in the array
	for(auto& particle : particleArray)
	{
		if( !particle.Alive() )
		{
			// insert from beginning of newArray
			particle = particles[preAllocated];
			preAllocated++;

			// Ran out particles to allocate
			if( preAllocated >= (particles.size()-1) ) return;
		}
	}

	// Insert the remaining particles at the end of the array
	particleArray.insert( particleArray.end(), particles.begin() + preAllocated, particles.end() );
}

void ParticleSystem::Update( float dt )
{
	for(auto& particle : particleArray)
	{
		if(particle.Alive() )
		{
			particle.Update( dt );
		}
	}
}

void ParticleSystem::Render( Camera& camera )
{
	for(auto& particle : particleArray)
	{
		if(particle.Alive() )
		{
			particle.Render( camera );
		}
	}
}
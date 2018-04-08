//-----------------------------------------------------------
// File: ParticleSystem.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "ParticleSystem.h"

const int ParticleSystem::BUFFER_SIZE = 20;

ParticleSystem::ParticleSystem()
{
	// reserve memory for the array to reduce excessive reallocation in the early game
	particleArray.reserve( BUFFER_SIZE );
}

ParticleSystem::~ParticleSystem()
{
	if( particleArray.size() > 0 )
	{
		particleArray.clear();
	}
}

void ParticleSystem::Reset()
{
	particleArray.clear();
}

void ParticleSystem::AddParticles( std::vector<Particle>& particles )
{
	// Index for the objects in "particles"
	int preAllocated = 0;

	// Attempt to allocate objects into existing space in the array
	for( int i = 0; i < particleArray.size(); i++ )
	{
		if( !particleArray[i].Alive() )
		{
			// insert from beginning of newArray
			particleArray[i] = particles[preAllocated];
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
	for( int i = 0; i < particleArray.size(); i++ )
	{
		if( particleArray[i].Alive() )
		{
			particleArray[i].Update( dt );
		}
	}
}

void ParticleSystem::Render( Camera& camera )
{
	for( int i = 0; i < particleArray.size(); i++ )
	{
		if( particleArray[i].Alive() )
		{
			particleArray[i].Render( camera );
		}
	}
}
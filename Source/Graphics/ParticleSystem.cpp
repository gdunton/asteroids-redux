//-----------------------------------------------------------
// File: ParticleSystem.cpp
//-----------------------------------------------------------

#include "ParticleSystem.h"

const int ParticleSystem::BUFFER_SIZE = 20;

ParticleSystem::ParticleSystem() noexcept
{
	// reserve memory for the array to reduce excessive reallocation in the early game
	particleArray.reserve(BUFFER_SIZE);
}

void ParticleSystem::Reset()
{
	particleArray.clear();
}

void ParticleSystem::AddParticles(std::vector<Particle>& newParticles)
{
	// Index for the objects in "particles"
	size_t preAllocated = 0;

	// Attempt to allocate objects into existing space in the array
	for(auto& particle : particleArray)
	{
		if(!particle.Alive())
		{
			// insert from beginning of newArray
			particle = newParticles.at(preAllocated);
			preAllocated++;

			// Ran out particles to allocate
			if(preAllocated >= (newParticles.size() - 1)) return;
		}
	}

	// Insert the remaining particles at the end of the array
	particleArray.insert(particleArray.end(), newParticles.begin() + preAllocated, newParticles.end());
}

void ParticleSystem::Update(float dt)
{
	for(auto& particle : particleArray)
	{
		if(particle.Alive())
		{
			particle.Update(dt);
		}
	}
}

void ParticleSystem::Render(const Camera& camera)
{
	for(auto& particle : particleArray)
	{
		if(particle.Alive())
		{
			particle.Render(camera);
		}
	}
}

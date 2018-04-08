//-----------------------------------------------------------
// File: Functions.h
// Description: Some functions that don't fit into any other place.
//-----------------------------------------------------------

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "..\Actor\Asteroid.h"
#include "..\Graphics\Particle.h"

// Converts an asteroid into a collection of particles for insertion into
// a particle system. Simplifies asteroid destruction
void AsteroidToParticles( Asteroid& asteroid, std::vector<Particle>& outVec );

// Converts level number into a game difficulty
Difficulty LevelToDifficulty( int levelNumber );

#endif
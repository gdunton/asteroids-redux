//-----------------------------------------------------------
// File: Functions.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Functions.h"

// Converts an asteroid into a collection of particles for insertion into
// a particle system. Simplifies asteroid destruction
std::vector<Particle> AsteroidToParticles(Asteroid& asteroid)
{
	// Convert each line in the asteroid into a particle
	const std::vector<Vector2>& points = asteroid.GetModel().GetPoints();

	std::vector<Particle> outVec;
	outVec.reserve(points.size());

	for(size_t i = 0; i < points.size(); ++i)
	{
		Vector2 p1 = asteroid.GetWorld().TransformPoint(points[i]);
		Vector2 p2 = asteroid.GetWorld().TransformPoint(points[(i + 1) % points.size()]);
		// Remove the position
		p1 -= asteroid.GetPos();
		p2 -= asteroid.GetPos();

		// Particle pos
		Vector2 pos = 0.5f * (p1 + p2);
		p1 = p1 - pos; // new object space
		p2 = p2 - pos;

		Vector2 direction = pos;
		Normalize(direction);
		Vector2 velocity = asteroid.GetVelocity() + (direction * Random() * 2);

		// Create a rotation value for each particle
		float rand = Random();
		rand *= 2;
		rand -= 1;

		Particle p(p1, p2, pos + asteroid.GetPos(), Vector2(1, 1), velocity, 0, (PI / 10) * rand, 2);
		outVec.push_back(p);
	}
	return outVec;
}

// Converts level number into a game difficulty
Difficulty LevelToDifficulty(int levelNumber)
{
	if(levelNumber == 1)
	{
		return EASY;
	}
	if(levelNumber > 1 && levelNumber <= 3)
	{
		return NORMAL;
	}
	if(levelNumber > 3 && levelNumber <= 5)
	{
		return HARD;
	}
	if(levelNumber > 5)
	{
		return EXTREME;
	}
	return NORMAL;
}

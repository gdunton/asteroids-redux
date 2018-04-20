//-----------------------------------------------------------
// File: ModelManager.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "ModelManager.h"

//#define INVERT_MODEL_Y
#ifdef INVERT_MODEL_Y
#define Vector2(x,y) Vector2(x,y*-1)
#endif

Model2D ModelManager::CreateAsteroidModel1() const
{
	return Model2D({
		Vector2(-1.9f, 2.2f),
		Vector2(-2.7f, 1.5f),
		Vector2(-2.9f, 0.0f),
		Vector2(-0.5f, -3.0f),
		Vector2(1.7f, -2.0f),
		Vector2(2.1f, 0.0f),
		Vector2(1.9f, 0.7f),
		Vector2(1.4f, 3.0f),
		Vector2(0.0f, 3.3f),
		Vector2(-1.9f, 2.2f)
	});
}

Model2D ModelManager::CreateAsteroidModel2() const
{
	return Model2D({
		Vector2(-2.0f, 3.2f),
		Vector2(-3.1f, 2.0f),
		Vector2(-3.3f, 0.0f),
		Vector2(-3.1f, -2.2f),
		Vector2(-1.4f, -2.7f),
		Vector2(0.0f, -3.0f),
		Vector2(2.0f, -2.4f),
		Vector2(3.0f, -1.0f),
		Vector2(3.1f, 1.5f),
		Vector2(2.8f, 1.9f),
		Vector2(-0.5f, 3.0f),
		Vector2(-2.0f, 3.2f)
	});
}

Model2D ModelManager::CreateAsteroidModel3() const
{
	return Model2D({
		Vector2(-0.3f, 3.1f),
		Vector2(-1.8f, 2.3f),
		Vector2(-2.2f, 0.7f),
		Vector2(-2.4f, -1.0f),
		Vector2(-1.4f, -2.3f),
		Vector2(-0.1f, -2.1f),
		Vector2(1.1f, -1.4f),
		Vector2(2.6f, -0.6f),
		Vector2(3.0f, 1.0f),
		Vector2(1.8f, 2.0f),
		Vector2(0.7f, 3.0f),
		Vector2(-0.3f, 3.1f)
	});
}

Model2D ModelManager::CreatePlayerModel() const
{
	return Model2D({
		Vector2(-1.0f, -1.0f),
		Vector2(0.0f, 2.0f),
		Vector2(1.0f, -1.0f),
		Vector2(-1.0f, -1.0f)
	});
}

Model2D ModelManager::CreateQuadModel() const
{
	return Model2D({
		Vector2(-1.0f, -1.0f),
		Vector2(-1.0f, 1.0f),
		Vector2(1.0f, 1.0f),
		Vector2(1.0f, -1.0f),
		Vector2(-1.0f, -1.0f)
	});
}

Model2D ModelManager::CreateCircle() const
{
	return Model2D({
		Vector2(1.0f, 0.0f),
		Vector2(0.966f, 0.259f),
		Vector2(0.866f, 0.5f),
		Vector2(0.707f, 0.707f),
		Vector2(0.5f, 0.866f),
		Vector2(0.259f, 0.966f),
		Vector2(0.0f, 1.0f),
		Vector2(-0.259f, 0.966f),
		Vector2(-0.5f, 0.866f),
		Vector2(-0.707f, 0.707f),
		Vector2(-0.866f, 0.5f),
		Vector2(-0.966f, 0.259f),
		Vector2(-1.0f, 0.0f),
		Vector2(-0.966f, -0.259f),
		Vector2(-0.866f, -0.5f),
		Vector2(-0.707f, -0.707f),
		Vector2(-0.5f, -0.866f),
		Vector2(-0.259f, -0.966f),
		Vector2(0.0f, -1.0f),
		Vector2(0.259f, -0.966f),
		Vector2(0.5f, -0.866f),
		Vector2(0.707f, -0.707f),
		Vector2(0.866f, -0.5f),
		Vector2(0.966f, -0.259f),
		Vector2(1.0f, 0.0f)
	});
}

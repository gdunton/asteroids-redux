//-----------------------------------------------------------
// File: Particle.cpp
//-----------------------------------------------------------

#include "Particle.h"

#include "Camera.h"
#include <algorithm>

Particle::Particle(Vector2 p1, Vector2 p2, Vector2 pos, Vector2 scale, Vector2 velocity,
                   float rotation, float rotationSpeed, float lifespan) :
	lifespan(lifespan),
	rotation(rotation),
	scale(std::move(scale)),
	pos(std::move(pos)),
	velocity(std::move(velocity))
{
	points = std::vector<Vector2>(2, Vector2());
	points[0] = p1;
	points[1] = p2;
	rotSpeed = rotationSpeed;
	initialLife = lifespan;

	alpha = 255;
}

void Particle::Update(float dt)
{
	pos += velocity * dt;

	rotation += rotSpeed * dt;
	scale *= ScaleRate;

	lifespan -= dt;

	// Update the alpha to fade out as the particle dies
	if(lifespan > 0)
	{
		alpha = 255 * (lifespan / initialLife);
	}
	else
	{
		alpha = 0;
	}

	// Reset the color of the particle with the alpha
	//line.SetColor(ColorRGBA(255, 255, 255, alpha));
	line.SetColor(ColorRGBA(255, 255, 255, 255));
}

void Particle::Render(const Camera& camera) const
{
	std::vector<Vector2> screenPoints;
	screenPoints.reserve(points.size());

	std::transform(points.begin(), points.end(), std::back_inserter(screenPoints),
	               [&camera, world = World(pos, scale, rotation)](const Vector2& p)
	               {
		               auto worldPoint = world.TransformPoint(p);
		               return camera.Transform(worldPoint);
	               });

	line.Render(screenPoints);
}

bool Particle::Alive() const
{
	return (lifespan > 0);
}

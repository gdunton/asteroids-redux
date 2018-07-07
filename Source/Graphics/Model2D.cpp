//-----------------------------------------------------------
// File: Model2D.cpp
//-----------------------------------------------------------

#include "Model2D.h"
#include <numeric>
#include "../Utilities/MathTypes.h"
#include "Camera.h"
#include <algorithm>

Model2D::Model2D(std::vector<Vector2> points) :
	points(std::move(points)),
	modelRadius(ModelStats())
{
	line.Initialize(WHITE);
}

void Model2D::Render(const World& world, const Camera& camera) const
{
	std::vector<Vector2> screenPoints;
	screenPoints.reserve(points.size());

	std::transform(points.begin(), points.end(), std::back_inserter(screenPoints),
	               [&world, &camera](const Vector2& p)
	               {
		               auto worldPoint = world.TransformPoint(p);
		               return camera.Transform(worldPoint);
	               });

	line.Render(screenPoints);
}

void Model2D::RenderToScreen(const World& world) const
{
	// Object already in screen pos so no need for a camera
	std::vector<Vector2> screenPoints;
	screenPoints.reserve(points.size());

	std::transform(points.begin(), points.end(), std::back_inserter(screenPoints),
	               [&world](const Vector2& p)
	               {
		               return world.TransformPoint(p);
	               });

	line.Render(screenPoints);
}

float Model2D::ModelStats()
{
	return std::accumulate(points.begin(), points.end(), 0.0f,
	                       [](float currentTotal, const Vector2& point)
	                       {
		                       return currentTotal += point.Length();
	                       }) / points.size();
}

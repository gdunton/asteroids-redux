//-----------------------------------------------------------
// File: Model2D.cpp
//-----------------------------------------------------------

#include "Model2D.h"
#include <numeric>
#include "../Utilities/MathTypes.h"

Model2D::Model2D(std::vector<Vector2> points) :
	points(std::move(points)),
	modelRadius(ModelStats())
{
	line.Initialize(WHITE);
}

void Model2D::Render( Camera& camera, const World& world ) const
{
	line.Render( &camera, points, world );
}

void Model2D::Render(const Vector2& screenPosition, const Vector2& scale, float rotation) const
{
	// Object already in screen pos so no need for a camera
	line.Render(nullptr, points, World(screenPosition, scale, rotation) );
}

float Model2D::ModelStats()
{
	return std::accumulate(points.begin(), points.end(), 0.0f,
		[](float currentTotal, const Vector2& point)
	{
		return currentTotal += point.Length();
	}) / points.size();
}
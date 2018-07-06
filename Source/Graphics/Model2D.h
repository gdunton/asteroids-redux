//-----------------------------------------------------------
// File: Model2D.h
// Description: Model class for the game. Uses a line to draw 
//				each model to screen.
//-----------------------------------------------------------

#ifndef MODEL_H
#define MODEL_H

#include "Line.h"

class Camera;

class Model2D
{
public:
	Model2D() = default;
	explicit Model2D(std::vector<Vector2> points);

	void Render(const World& world, const Camera& camera) const;
	void RenderToScreen(const World& world) const;

	float GetModelRadius() const { return modelRadius; }

	const std::vector<Vector2>& GetPoints() const { return points; }

	void SetColor(Color color) { line.SetColor(color); }

private:
	std::vector<Vector2> points;
	Line line;

	float modelRadius = 0;

	// Calculates the radius of the model
	float ModelStats();
};

#endif

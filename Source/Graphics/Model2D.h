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
	friend class ModelManager;

private:
	std::vector<Vector2> points;
	Line line;

	float modelRadius;

public:
	Model2D();
	~Model2D();

	void Render( Camera& camera, const World& world );
	void Render(const Vector2& screenPosition, const Vector2& scale, float rotation);

	// Calculates the radius of the model
	void CalculateModelStats();

	float GetModelRadius() { return modelRadius; }

	const std::vector<Vector2>& GetPoints() { return points; }

	void SetColor( Color color ) { line.SetColor( color ); }
};

#endif
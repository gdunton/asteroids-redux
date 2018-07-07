//-----------------------------------------------------------
// File: Camera.cpp
//-----------------------------------------------------------

#include "Camera.h"

Camera::Camera(Vector2 center, Vector2 size,
               float windowWidth, float windowHeight) :
	center(std::move(center)),
	size(std::move(size)),
	windowWidth(static_cast<int>(windowWidth)),
	windowHeight(static_cast<int>(windowHeight))
{}

Vector2 Camera::Transform(const Vector2& worldPos) const
{
	// move the world pos into relative camera space
	// screen pos = world pos / cam size + 0.5 (remember to invert Y)

	Vector2 camRelPos = worldPos - center;
	camRelPos.y = (-camRelPos.y / size.y) + 0.5f;
	camRelPos.x = (camRelPos.x / size.x) + 0.5f;
	return Vector2(windowWidth * camRelPos.x, windowHeight * camRelPos.y);
}

MathTypes::Rectangle Camera::GetScreenRect() const
{
	const Vector2 offset = size * 1.05f;
	return MathTypes::Rectangle(center - static_cast<Vector2>(size / 2) - offset, size + (offset * 2.0f));
}

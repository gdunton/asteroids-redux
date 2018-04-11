//-----------------------------------------------------------
// File: Camera.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Camera.h"

Camera::Camera()
{
	center = Vector2( 0, 0 );
	size = Vector2( 0, 0 );

	windowWidth = 0;
	windowHeight = 0;
}

Camera::~Camera()
{
}

void Camera::Initialize(const Vector2& _center, const Vector2& _size,
                        float _windowWidth, float _windowHeight)
{
	center = _center;
	size = _size;
	windowWidth = static_cast<int>(_windowWidth);
	windowHeight = static_cast<int>(_windowHeight);
}

Vector2 Camera::ConvertWorldToScreenPos( const Vector2& wPos ) const
{
	// move the world pos into relative camera space
	// screen pos = world pos / cam size + 0.5 (remember to invert Y)

	Vector2 camRelPos = wPos - center;
	camRelPos.y = (-camRelPos.y / size.y) + 0.5f;
	camRelPos.x = (camRelPos.x / size.x) + 0.5f;
	return Vector2( windowWidth * camRelPos.x, windowHeight * camRelPos.y );
}

MathTypes::Rectangle Camera::GetScreenRect() const
{
	Vector2 offset = size * 1.05f;
	return MathTypes::Rectangle( center - static_cast<Vector2>(size / 2) - offset, size + (offset*2.0f) );
}
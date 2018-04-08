//-----------------------------------------------------------
// File: Camera.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Camera.h"

Camera::Camera()
{
	center = MyVector2( 0, 0 );
	size = MyVector2( 0, 0 );

	windowWidth = 0;
	windowHeight = 0;
}

Camera::~Camera()
{
}

void Camera::Initialize( const MyVector2& _center, const MyVector2& _size, 
	const int _windowWidth, const int _windowHeight )
{
	center = _center;
	size = _size;
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;
}

MyVector2 Camera::ConvertWorldToScreenPos( const MyVector2& wPos ) const
{
	// move the world pos into relative camera space
	// screen pos = world pos / cam size + 0.5 (remember to invert Y)

	MyVector2 camRelPos = wPos - center;
	camRelPos.y = (-camRelPos.y / size.y) + 0.5f;
	camRelPos.x = (camRelPos.x / size.x) + 0.5f;
	return MyVector2( windowWidth * camRelPos.x, windowHeight * camRelPos.y );
}

MathTypes::Rectangle Camera::GetScreenRect() const
{
	MyVector2 offset = size * 1.05f;
	return MathTypes::Rectangle( center - static_cast<MyVector2>(size / 2) - offset, size + (offset*2.0f) );
}
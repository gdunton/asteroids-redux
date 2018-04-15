//-----------------------------------------------------------
// File: Camera.h
// Description: Camera used for converting world positions and 
//				into screen positions
//-----------------------------------------------------------

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:

	Camera(const Vector2& center, const Vector2& size, float windowWidth, float windowHeight);

	// Takes world position and outputs a point with coords in the client dimensions
	Vector2 ConvertWorldToScreenPos( const Vector2& worldPos ) const;

	// Get rectangle to help find objects in camera view
	MathTypes::Rectangle GetScreenRect() const;

	const Vector2& GetCenter() const { return center; }
	const Vector2& GetSize() const { return size; }

	void SetCenter( const Vector2& _center ) { center = _center; }
	void SetSize( const Vector2& _size ) { size = _size; }

private:
	Vector2 center;
	Vector2 size;

	// The window client dimensions
	int windowWidth = 0;
	int windowHeight = 0;
};

#endif
//-----------------------------------------------------------
// File: Camera.h
// Description: Camera used for converting world positions and 
//				into screen positions
//-----------------------------------------------------------

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
private:
	MyVector2 center;
	MyVector2 size;

	// The window client dimensions
	int windowWidth;
	int windowHeight;

public:
	Camera();
	~Camera();

	void Initialize( const MyVector2& center, const MyVector2& size, const int windowWidth, const int windowHeight );

	// Takes world position and outputs a point with coords in the client dimensions
	MyVector2 ConvertWorldToScreenPos( const MyVector2& worldPos ) const;

	// Get rectangle to help find objects in camera view
	MathTypes::Rectangle GetScreenRect() const;

	const MyVector2& GetCenter() const { return center; }
	const MyVector2& GetSize() const { return size; }

	void SetCenter( const MyVector2& _center ) { center = _center; }
	void SetSize( const MyVector2& _size ) { size = _size; }
};

#endif
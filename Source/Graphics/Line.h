//-----------------------------------------------------------
// File: Line.h
// Description: Container for DirectX Line. Used to draw
//				models on screen
//-----------------------------------------------------------

#ifndef LINE_H
#define LINE_H

#include <PrimitiveBatch.h>
#include <VertexTypes.h>

class Camera;

using Batch = DirectX::PrimitiveBatch<DirectX::VertexPositionColor>;

class Line
{
private:
	static Batch* m_pLine;
	Color m_color;

public:
	Line();

	static void SetLine(Batch* line);

	// Initialize creates the line interface from DX and sets the color
	void Initialize( const Color color );
	void Render( Camera* camera, const std::vector<Vector2>& points, const World& world ) const;

	void SetColor( const Color _color ) { m_color = _color; }
};

#endif
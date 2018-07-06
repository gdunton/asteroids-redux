//-----------------------------------------------------------
// File: Line.h
// Description: Container for DirectX Line. Used to draw
//				models on screen
//-----------------------------------------------------------

#ifndef LINE_H
#define LINE_H

#include <vector>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include "../Utilities/Color.h"
#include "../Utilities/Vector2.h"

struct World;
class Camera;

using Batch = DirectX::PrimitiveBatch<DirectX::VertexPositionColor>;

class Line
{
public:
	Line();

	static void SetLine(Batch* line);

	// Initialize creates the line interface from DX and sets the color
	void Initialize(Color color);
	void Render(const std::vector<Vector2>& points) const;

	void SetColor(const Color _color) { m_color = _color; }

private:
	static Batch* m_pLine;
	Color m_color;
};

#endif

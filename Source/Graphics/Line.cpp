//-----------------------------------------------------------
// File: Line.cpp
//-----------------------------------------------------------

#include <algorithm>
#include "../Utilities/MathTypes.h"
#include "Line.h"
#include "Camera.h"
#include "GraphicsDeviceManager.h"

Batch* Line::m_pLine = nullptr;

Line::Line() :
	m_color(WHITE)
{}

void Line::SetLine(Batch* line)
{
	m_pLine = line;
}

void Line::Initialize(const Color color)
{
	SetColor(color);
}

void Line::Render(const std::vector<Vector2>& points) const
{
	std::vector<DirectX::VertexPositionColor> outPoints;
	outPoints.reserve(points.size());
	std::transform(points.begin(), points.end(), std::back_inserter(outPoints), [color = m_color](Vector2 point)
	{
		return DirectX::VertexPositionColor{ DirectX::XMFLOAT3{ point.x, point.y, 0 }, color };
	});

	// Draw all points
	m_pLine->Begin();
	m_pLine->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, &outPoints[0], outPoints.size());
	m_pLine->End();
}

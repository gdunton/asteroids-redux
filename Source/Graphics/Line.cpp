//-----------------------------------------------------------
// File: Line.cpp
//-----------------------------------------------------------

#include "STD.h"
#include <algorithm>
#include "Line.h"
#include "Camera.h"

Batch* Line::m_pLine = nullptr;

Line::Line() :
	m_color(WHITE)
{
}

void Line::SetLine(Batch* line) 
{
	m_pLine = line;
}

void Line::Initialize( const Color color )
{
	SetColor( color );
}

void Line::Render( Camera* camera, const std::vector<MyVector2>& points, const World& world )
{
	std::vector<DirectX::VertexPositionColor> outPoints;
	outPoints.reserve(points.size());
	std::transform(points.begin(), points.end(), std::back_inserter(outPoints), [&world, camera, color = m_color](MyVector2 point)
	{
		point = world.TransformPoint(point);
		if (camera != nullptr)
		{
			point = camera->ConvertWorldToScreenPos(point);
		}
		return DirectX::VertexPositionColor{ DirectX::XMFLOAT3{ point.x, point.y, 0 }, color };
	});

	// Draw all points
	m_pLine->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, &outPoints[0], outPoints.size());
}
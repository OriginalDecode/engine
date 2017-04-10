#include "stdafx.h"
#include "Quad.h"


void Quad::Initiate()
{
	//m_InputLayout = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};


	m_VertexData.myNrOfVertexes = 4;
	m_VertexData.myStride = sizeof(VertexTypePosUV);
	m_VertexData.mySize = m_VertexData.myNrOfVertexes * m_VertexData.myStride;
	m_VertexData.myVertexData = new s8[m_VertexData.mySize];

	CU::GrowingArray<VertexTypePosUV> vertices;
	CU::GrowingArray<s32> indices;

	VertexTypePosUV vertex;
	vertex.myPosition = { -1, -1, 0 };
	vertex.myUV = { 0, 1 };
	vertices.Add(vertex);

	vertex.myPosition = { -1, 1, 0 };
	vertex.myUV = { 0, 0 };
	vertices.Add(vertex);


	vertex.myPosition = { 1, -1, 0 };
	vertex.myUV = { 1, 1 };
	vertices.Add(vertex);


	vertex.myPosition = { 1, 1, 0 };
	vertex.myUV = { 1, 0 };
	vertices.Add(vertex);
	
	memcpy(m_VertexData.myVertexData, &vertices[0], m_VertexData.mySize);

	indices.Add(0);
	indices.Add(1);
	indices.Add(2);

	indices.Add(3);
	indices.Add(2);
	indices.Add(1);

	m_IndexData.myFormat = DXGI_FORMAT_R32_UINT;
	m_IndexData.myIndexCount = 6;
	m_IndexData.mySize = m_IndexData.myIndexCount * 4;

	//m_IndexData.myIndexData = new s8[m_IndexData->mySize];
	//memcpy(myIndexData->myIndexData, &indices[0], myIndexData->mySize);



}

#pragma once

#include "snowblind_shared.h"
#include <DataStructures/GrowingArray.h>
#include <Engine/VertexWrapper.h>
#include <Engine/IndexWrapper.h>
#include <Engine/VertexStructs.h>

class Quad
{
public:
	Quad() = default;

	void Initiate();


private:

	IInputLayout* m_InputLayout = nullptr;
	CU::GrowingArray<VertexTypePosUV> m_Vertices;
	D3D11_INPUT_ELEMENT_DESC m_VertexFormat[2];
	VertexBufferWrapper m_VertexBuffer;
	VertexDataWrapper m_VertexData;
	IndexBufferWrapper m_IndexBuffer;
	IndexDataWrapper m_IndexData;
};


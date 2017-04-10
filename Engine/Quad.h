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

	void SetBuffers();
	void Render();

private:

	IInputLayout* m_InputLayout = nullptr;
	CU::GrowingArray<VertexTypePosUV> m_Vertices;
	VertexBufferWrapper m_VertexBuffer;
	VertexDataWrapper m_VertexData;
	IndexBufferWrapper m_IndexBuffer;
	IndexDataWrapper m_IndexData;
	Effect* m_ScreenpassShader = nullptr;
};


#pragma once

#include "engine_shared.h"
#include <DataStructures/GrowingArray.h>
#include <Engine/VertexWrapper.h>
#include <Engine/IndexWrapper.h>
#include <Engine/VertexStructs.h>

class Quad
{
public:
	Quad() = default;

	void Initiate();
	void CleanUp();
	void SetBuffers();
	void Render();
	Effect* GetShader() { return m_ScreenpassShader; }

private:

	IInputLayout* m_InputLayout = nullptr;
	CU::GrowingArray<VertexTypePosUV> m_Vertices;
	VertexBufferWrapper m_VertexBuffer;
	VertexDataWrapper m_VertexData;
	IndexBufferWrapper m_IndexBuffer;
	IndexDataWrapper m_IndexData;
	Effect* m_ScreenpassShader = nullptr;
};


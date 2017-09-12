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
	void Render();
	Effect* GetShader() { return m_ScreenpassShader; }

	const VertexWrapper& GetVertexWrapper() const { return m_VertexWrapper; }
	const IndexWrapper& GetIndexWrapper() const { return m_IndexWrapper; }

private:
	CU::GrowingArray<VertexTypePosUV> m_Vertices;

	VertexWrapper m_VertexWrapper;
	IndexWrapper m_IndexWrapper;

	Effect* m_ScreenpassShader = nullptr;
};


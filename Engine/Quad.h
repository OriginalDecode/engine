#pragma once

#include "engine_shared.h"
#include <DataStructures/GrowingArray.h>
#include <Engine/VertexWrapper.h>
#include <Engine/IndexWrapper.h>
#include <Engine/VertexStructs.h>

class Quad
{
public:
	Quad(Effect* effect);
	void Render();
	Effect* GetShader() { return m_Effect; }
	const VertexWrapper& GetVertexWrapper() const { return m_VertexWrapper; }
	const IndexWrapper& GetIndexWrapper() const { return m_IndexWrapper; }

	void SetTexture(Texture* texture) { m_QuadTexture = texture; }

private:
	VertexWrapper m_VertexWrapper;
	IndexWrapper m_IndexWrapper;
	Texture* m_QuadTexture = nullptr;
	Effect* m_Effect = nullptr;
};


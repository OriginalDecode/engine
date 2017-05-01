#include "stdafx.h"
#include "BaseModel.h"
#include <stdio.h>
#ifdef _PROFILE
#include <easy/profiler.h>
#endif

CBaseModel::~CBaseModel() = default;

void CBaseModel::SetupLayoutsAndBuffers()
{
	Engine::GetAPI()->GetContext()->IASetInputLayout(m_VertexLayout);
	Engine::GetAPI()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Engine::GetAPI()->GetContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer.myVertexBuffer, &m_VertexBuffer.myStride, &m_VertexBuffer.myByteOffset);
	Engine::GetAPI()->GetContext()->IASetIndexBuffer(m_IndexBuffer.myIndexBuffer, DXGI_FORMAT_R32_UINT, m_IndexBuffer.myByteOffset);
}

void CBaseModel::SetEffect(Effect* anEffect)
{
	myEffect = anEffect;
}

void CBaseModel::InitVertexBuffer()
{
	void* shader = myEffect->GetVertexShader()->compiledShader;
	s32 size = myEffect->GetVertexShader()->shaderSize;
	m_VertexLayout = Engine::GetAPI()->CreateInputLayout(shader, size, &myVertexFormat[0], myVertexFormat.Size());

#ifdef _DEBUG
	Engine::GetAPI()->SetDebugName(m_VertexLayout, "Model Vertex Layout : " + m_Filename);
#endif
	m_VertexBuffer.myVertexBuffer = Engine::GetAPI()->CreateBuffer(m_VertexData.mySize, m_VertexData.myVertexData);
#ifdef _DEBUG

	Engine::GetAPI()->SetDebugName(m_VertexBuffer.myVertexBuffer, "Model Vertex Buffer " + m_Filename);
#endif

	m_VertexBuffer.myStride = m_VertexData.myStride;
	m_VertexBuffer.myByteOffset = 0;
	m_VertexBuffer.myStartSlot = 0;
	m_VertexBuffer.myNrOfBuffers = 1;

}

void CBaseModel::InitIndexBuffer()
{
	m_IndexBuffer.myIndexBuffer = Engine::GetAPI()->CreateBuffer(m_IndexData.mySize, m_IndexData.myIndexData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER);

	Engine::GetAPI()->SetDebugName(m_IndexBuffer.myIndexBuffer, "Model Index Buffer " + m_Filename);
	m_IndexBuffer.myIndexBufferFormat = m_IndexData.myFormat;
	m_IndexBuffer.myByteOffset = 0;
}

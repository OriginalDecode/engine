#pragma once
#include "VertexStructs.h"
#include "Utilities.h"
#include <Engine/IGraphicsAPI.h>
#include <Engine/ConstantBuffer.h>
struct SFontData;
class Effect; 
namespace graphics
{
	class RenderContext;
}

class CFont
{
public:
	CFont(SFontData* aFontData);
	~CFont();

	void SetText(std::string aText);
	const std::string& GetText() const;
	void Render(const graphics::RenderContext& rc);

	void SetPosition(const CU::Vector2f& aPosition);
	void SetScale(const CU::Vector2f& aScale);
	void SetMatrices(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix);
	VertexWrapper& GetVertexWrapper() { return m_VertexWrapper; }
	IndexWrapper& GetIndexWrapper() { return m_IndexWrapper; }

	const short& GetFontSize() const;
private:
	void operator=(const CFont&) = delete;
	
	void UpdateBuffer();
	void UpdateConstantBuffer();
	SFontData* m_Data = nullptr;
	Effect* m_Effect[2];

	std::string m_Text = "NONE";

 	CU::Vector2f m_Size;
 
 	CU::GrowingArray<SVertexTypePosColUv> m_Vertices;
 	CU::GrowingArray<s32> m_Indices;
 
 	cl::SColor m_DefaultColor;
 	cl::SColor m_Color;
	VertexWrapper m_VertexWrapper;
	IndexWrapper m_IndexWrapper;


	graphics::ConstantBuffer m_Buffer;
	CU::Vector2f m_Position;// = CU::Vector2f(0.f, 0.f);
	CU::Vector2f m_Scale;// = CU::Vector2f(1.f, 1.f);
/*
	IBuffer* m_cbFont = nullptr;
	struct SFontConstantBuffer : public VertexBaseStruct
	{
		CU::Vector2f position = CU::Vector2f(0.f, 0.f);
		CU::Vector2f scale = CU::Vector2f(1.f, 1.f);
	} myConstantStruct;
*/

	graphics::BufferDesc m_VertexDesc;
	graphics::BufferDesc m_IndexDesc;

};

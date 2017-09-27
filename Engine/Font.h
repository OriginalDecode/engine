#pragma once
#include "VertexStructs.h"
#include "Utilities.h"
#include <Engine/IGraphicsAPI.h>

struct SFontData;
class Effect;
class CFont
{
public:
	CFont(SFontData* aFontData);
	~CFont();

	void SetText(std::string aText);
	const std::string& GetText() const;
	void Render();

	void SetPosition(const CU::Vector2f& aPosition);
	void SetScale(const CU::Vector2f& aScale);
	void SetMatrices(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix);
	const VertexWrapper& GetVertexWrapper() const { return m_VertexWrapper; }
	const IndexWrapper& GetIndexWrapper() const { return m_IndexWrapper; }

	const short& GetFontSize() const { return m_Data->myFontHeightWidth; }
private:
	void operator=(const CFont&) = delete;
	
	void UpdateBuffer();
	void UpdateConstantBuffer();
	SFontData* m_Data = nullptr;
	Effect* m_Effect[2];

	std::string m_Text = "NONE";

 	CU::Vector2f m_Size;
 
 	CU::GrowingArray<SVertexTypePosColUv> m_Vertices;
 	CU::GrowingArray<int> m_Indices;
 
 	cl::SColor m_DefaultColor;
 	cl::SColor m_Color;
	VertexWrapper m_VertexWrapper;
	IndexWrapper m_IndexWrapper;

	IBuffer* m_cbFont = nullptr;
	struct SFontConstantBuffer : public VertexBaseStruct
	{
		CU::Vector2f position = CU::Vector2f(0.f, 0.f);
		CU::Vector2f scale = CU::Vector2f(1.f, 1.f);
	} myConstantStruct;


	graphics::BufferDesc m_VertexDesc;
	graphics::BufferDesc m_IndexDesc;

};

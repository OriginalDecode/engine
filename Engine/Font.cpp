#include "stdafx.h"
#include "Font.h"
#include "FontManager.h"
#include "AssetsContainer.h"

#include <Engine/VertexWrapper.h>
#include <Engine/IndexWrapper.h>


CFont::CFont(SFontData* aFontData)
{
	m_Data = aFontData;
	m_Text = " ";
	m_Effect[0] = Engine::GetInstance()->GetEffect("Shaders/font_outline.json");
	m_Effect[0]->AddShaderResource(m_Data->m_AtlasView, Effect::DIFFUSE);

	m_Effect[1] = Engine::GetInstance()->GetEffect("Shaders/font.json");
	m_Effect[1]->AddShaderResource(m_Data->m_AtlasView, Effect::DIFFUSE);

	m_cbFont = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(SFontConstantBuffer), "Font ConstantBuffer");


	graphics::InputElementDesc layout[] =
	{
		{ "POSITION", 0, graphics::_12BYTE_RGB, 0, 0, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, graphics::_16BYTE_RGBA, 0, 12, graphics::INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, graphics::_8BYTE_RG, 0, 28, graphics::INPUT_PER_VERTEX_DATA, 0 }

	};
	IInputLayout* input_layout = Engine::GetAPI()->GetDevice().CreateInputLayout(m_Effect[0]->GetVertexShader(), layout, ARRSIZE(layout));


	const s32 vtx_stride = sizeof(SVertexTypePosColUv);
	const s32 vtx_byte_offset = 0;
	const s32 vtx_start_slot = 0;
	const s32 vtx_buffer_count = 1;

	m_VertexWrapper = VertexWrapper(nullptr, vtx_start_slot, vtx_buffer_count, vtx_stride, vtx_byte_offset, 0, 0, nullptr, input_layout, graphics::TRIANGLE_LIST);

	m_VertexDesc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	m_VertexDesc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	m_VertexDesc.m_CPUAccessFlag = graphics::WRITE;


	const graphics::eTextureFormat format = graphics::R32_UINT;
	const s32 idx_byte_offset = 0;

	m_IndexDesc.m_UsageFlag = graphics::IMMUTABLE_USAGE;
	m_IndexDesc.m_BindFlag = graphics::BIND_INDEX_BUFFER;
	m_IndexDesc.m_CPUAccessFlag = graphics::NO_ACCESS_FLAG;

	m_DefaultColor.SetRGB(255, 255, 255);
	m_DefaultColor.SetA(255);
	m_Color = m_DefaultColor;


}

CFont::~CFont()
{
	Engine::GetAPI()->ReleasePtr(m_cbFont);
}

void CFont::SetText(std::string aText)
{
	if (m_Text != aText)
	{
		m_Text = aText;
		UpdateBuffer();
	}
}

const std::string& CFont::GetText() const
{
	return m_Text;
}

void CFont::Render()
{
	if (!m_Effect[0] || !m_Effect[1])
		return;

	auto& api = *Engine::GetAPI();
	auto& ctx = Engine::GetAPI()->GetContext();

	ctx.SetBlendState(api.GetBlendState(graphics::ALPHA_BLEND));
	ctx.PSSetSamplerState(0, 1, Engine::GetInstance()->GetActiveSampler());
	UpdateConstantBuffer();

	ctx.DrawIndexed(this, m_Effect[0]);
	ctx.DrawIndexed(this, m_Effect[1]);

}


void CFont::SetPosition(const CU::Vector2f& aPosition)
{
	myConstantStruct.position = aPosition;
}

void CFont::SetScale(const CU::Vector2f& aScale)
{
	myConstantStruct.scale = aScale;
}

void CFont::SetMatrices(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix)
{
	myConstantStruct.world = anOrientation;
	myConstantStruct.invertedView = CU::Math::Inverse(a2DCameraOrientation);
	myConstantStruct.projection = anOrthogonalProjectionMatrix;
}

const short& CFont::GetFontSize() const
{
	return m_Data->myFontHeightWidth;
}

void CFont::UpdateBuffer()
{
	m_Color = m_DefaultColor;

	u32 count = u32(m_Text.length());
	float z = 0.f;
	float drawX = 0.f;
	float drawY = -5.f;
	float maxDrawY = 0.f;
	m_Vertices.RemoveAll();
	m_Indices.RemoveAll();

	SVertexTypePosColUv v;
	u32 skips = 0;
	for (u32 i = 0, row = 0; i < count; i++)
	{
		SCharData& charData = m_Data->myCharData[m_Text[i]];

		if (maxDrawY < charData.myHeight)
		{
			maxDrawY = charData.myHeight;
		}

		if (m_Text[i] == '\n')
		{
			drawX = 0;
			drawY -= (maxDrawY + 6);
			row++;
			continue;
		}

		if (m_Text[i] == '#')
		{
			skips = 0;
			if (m_Text[i + 2] != 'x')
			{
				std::string hex_code = "0x" + m_Text.substr(i + 1, 6) + "FF";
				unsigned int _color = (unsigned int)strtoul(hex_code.c_str(), nullptr, 16);
				m_Color.Convert(_color);
				i += 7;
				skips = 8;
				if (m_Text[i] != '(')
					m_Color = m_DefaultColor;
				continue;
			}
			else
			{
				std::string hex_code = m_Text.substr(i + 3, 6) + "FF"; //might extend this with a lerp alpha in the future?
				unsigned int _color = (unsigned int)strtoul(hex_code.c_str(), nullptr, 16);
				m_Color.Convert(_color);
				i += 9;
				skips = 10;
				continue;
			}


		}

		if (m_Text[i] == ')')
		{
			m_Color = m_DefaultColor;
			continue;
		}


		float left = drawX;
		float right = left + charData.myWidth;
		float top = drawY + charData.myBearingY;
		float bottom = top + charData.myHeight;

		v.myPosition = { left, bottom, z };
		v.myColor = m_Color.ToVec4();
		v.myUV = charData.myTopLeftUV;
		m_Vertices.Add(v);

		v.myPosition = { left, top, z };
		v.myColor = m_Color.ToVec4();
		v.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
		m_Vertices.Add(v);

		v.myPosition = { right, bottom, z };
		v.myColor = m_Color.ToVec4();
		v.myUV = { charData.myBottomRightUV.x, charData.myTopLeftUV.y };
		m_Vertices.Add(v);

		v.myPosition = { right, top, z };
		v.myColor = m_Color.ToVec4();
		v.myUV = charData.myBottomRightUV;
		m_Vertices.Add(v);

		u32 startIndex = (i - skips - row) * 4.f;

		m_Indices.Add(startIndex + 1);
		m_Indices.Add(startIndex + 0);
		m_Indices.Add(startIndex + 2);

		m_Indices.Add(startIndex + 2);
		m_Indices.Add(startIndex + 3);
		m_Indices.Add(startIndex + 1);

		drawX += charData.myBearingX;


	}

	if (m_Vertices.Size() <= 0)
		return;

	auto& device = Engine::GetAPI()->GetDevice();

	m_VertexDesc.m_ByteWidth = sizeof(SVertexTypePosColUv) * m_Vertices.Size();
	m_VertexDesc.m_Data = &m_Vertices[0];
	m_VertexWrapper.ReleaseBuffer();
	m_VertexWrapper.SetBuffer(device.CreateBuffer(m_VertexDesc, "Font VertexBuffer"));


	m_IndexDesc.m_ByteWidth = sizeof(u32) * m_Indices.Size();
	m_IndexDesc.m_Data = &m_Indices[0];
	m_IndexWrapper.ReleaseBuffer();
	m_IndexWrapper.SetBuffer(device.CreateBuffer(m_IndexDesc, "Font IndexBuffer"));
}

void CFont::UpdateConstantBuffer()
{
	auto& ctx = Engine::GetAPI()->GetContext();
	ctx.UpdateConstantBuffer(m_cbFont, &myConstantStruct, sizeof(SFontConstantBuffer));
	ctx.VSSetConstantBuffer(0, 1, &m_cbFont);

}


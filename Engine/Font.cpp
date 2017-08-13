#include "stdafx.h"
#include "Font.h"
#include "FontManager.h"

#include "VertexWrapper.h"
#include "IndexWrapper.h"

#include <TimeManager.h>
#include "AssetsContainer.h"


CFont::CFont(SFontData* aFontData)
{

	myEngine = Engine::GetInstance();
	myTimeManager = new CU::TimeManager();


	myUpdateTimer = myTimeManager->CreateTimer();
	myRenderTimer = myTimeManager->CreateTimer();

	myData = aFontData;
	myText = " ";
	myEffect[0] = myEngine->GetEffect("Shaders/font_outline.json");
	myEffect[1] = myEngine->GetEffect("Shaders/font.json");

	myVertexBufferDesc = new D3D11_BUFFER_DESC();
	myIndexBufferDesc = new D3D11_BUFFER_DESC();
	myInitData = new D3D11_SUBRESOURCE_DATA();

	CreateInputLayout();
	CreateBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();

	myDefaultColor.SetRGB(255, 255, 255);
	myDefaultColor.SetA(255);
	myColor = myDefaultColor;

	myRenderTime = 0.f;
	myUpdateTime = 0.f;

}

CFont::~CFont()
{
	SAFE_DELETE(myIndexBuffer);
	SAFE_DELETE(myVertexBuffer);

	SAFE_DELETE(myVertexBufferDesc);
	SAFE_DELETE(myIndexBufferDesc);
	SAFE_DELETE(myTimeManager);

	SAFE_DELETE(myInitData);
	SAFE_RELEASE(myVertexLayout);

	SAFE_DELETE(myConstantStruct);
	SAFE_RELEASE(myConstantBuffer);
}

void CFont::SetText(std::string aText)
{
	myTimeManager->GetTimer(myUpdateTimer).Update();
	myUpdateTime = myTimeManager->GetTimer(myUpdateTimer).GetTotalTime().GetMilliseconds();

	if (myText != aText)
	{
		myText = aText;
		UpdateBuffer();
	}

	myTimeManager->GetTimer(myUpdateTimer).Update();
	myUpdateTime = myTimeManager->GetTimer(myUpdateTimer).GetTotalTime().GetMilliseconds() - myUpdateTime;
}

const std::string& CFont::GetText() const
{
	return myText;
}

void CFont::Render()
{
	myTimeManager->GetTimer(myRenderTimer).Update();
	myRenderTime = myTimeManager->GetTimer(myRenderTimer).GetTotalTime().GetMilliseconds();

	if (!myEffect[0] || !myEffect[1])
		return;
	//myEffect->SetTexture(myData->myAtlasView, "FontTexture");

	Engine::GetAPI()->SetBlendState(eBlendStates::ALPHA_BLEND);
	Engine::GetAPI()->SetSamplerState(eSamplerStates::LINEAR_CLAMP);
	//CEngine::GetAPI()->SetRasterizer(eRasterizer::MSAA);

	ID3D11DeviceContext& context = *Engine::GetAPI()->GetContext();
	context.IASetInputLayout(myVertexLayout);
	context.IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNrOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
	context.IASetIndexBuffer(myIndexBuffer->myIndexBuffer, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);
	context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UpdateConstantBuffer();

	//Engine::GetAPI()->SetVertexShader(myEffect[0]->GetVertexShader()->m_Shader);
	context.VSSetConstantBuffers(0, 1, &myConstantBuffer);

	for (int i = 0; i < 2; i++)
	{
		myEffect[i]->Use();
		//Engine::GetAPI()->SetPixelShader(myEffect[i]->GetPixelShader()->m_Shader);
		ID3D11ShaderResourceView* srv = myData->myAtlasView;
		context.PSSetShaderResources(0, 1, &srv);
		context.DrawIndexed(myIndices.Size(), 0, 0);
		myEffect[i]->Clear();

	}

	myTimeManager->GetTimer(myRenderTimer).Update();
	myRenderTime = myTimeManager->GetTimer(myRenderTimer).GetTotalTime().GetMilliseconds() - myRenderTime;

	Engine::GetAPI()->SetBlendState(eBlendStates::NO_BLEND);
}

ID3D11ShaderResourceView* CFont::GetAtlas()
{
	return myData->myAtlasView;
}

const CU::Math::Vector2<float>& CFont::GetSize()
{
	return mySize;
}

const short CFont::GetFontPixelSize()
{
	return myData->myFontHeightWidth;
}

float CFont::GetUpdateTime()
{
	return myUpdateTime *1000.f;
}

float CFont::GetRenderTime()
{
	return myRenderTime *1000.f;
}

void CFont::SetPosition(const CU::Vector2f& aPosition)
{
	myConstantStruct->position = aPosition;
}

void CFont::SetScale(const CU::Vector2f& aScale)
{
	myConstantStruct->scale = aScale;
}

void CFont::SetMatrices(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix)
{
	myConstantStruct->world = anOrientation;
	myConstantStruct->invertedView = CU::Math::Inverse(a2DCameraOrientation);
	myConstantStruct->projection = anOrthogonalProjectionMatrix;
}

void CFont::CreateInputLayout()
{
	myVertexFormat.ReInit(3);
	myVertexFormat.Add(VertexLayoutPosColUV[0]);
	myVertexFormat.Add(VertexLayoutPosColUV[1]);
	myVertexFormat.Add(VertexLayoutPosColUV[2]);
	HRESULT hr = Engine::GetAPI()->GetDevice()->CreateInputLayout(&myVertexFormat[0]
		, myVertexFormat.Size()
		, myEffect[0]->GetVertexShader()->compiledShader
		, myEffect[0]->GetVertexShader()->shaderSize
		, &myVertexLayout);
	Engine::GetAPI()->HandleErrors(hr, " [Font] : Input Layout.");
	Engine::GetAPI()->SetDebugName(myVertexLayout, "Font Input Layout");
}

void CFont::CreateBuffer()
{
	myVertexBuffer = new VertexBufferWrapper;
	myVertexBuffer->myStride = sizeof(SVertexTypePosColUv);
	myVertexBuffer->myByteOffset = 0;
	myVertexBuffer->myStartSlot = 0;
	myVertexBuffer->myNrOfBuffers = 1;
	myVertexBuffer->myVertexBuffer = nullptr;

	ZeroMemory(myVertexBufferDesc, sizeof(*myVertexBufferDesc));
	myVertexBufferDesc->Usage = D3D11_USAGE_DYNAMIC;
	myVertexBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	myVertexBufferDesc->CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	myVertexBufferDesc->MiscFlags = 0;
	myVertexBufferDesc->StructureByteStride = 0;
}

void CFont::CreateIndexBuffer()
{
	myIndexBuffer = new IndexBufferWrapper;
	myIndexBuffer->myIndexBufferFormat = DXGI_FORMAT_R32_UINT;
	myIndexBuffer->myByteOffset = 0;
	myIndexBuffer->myIndexBuffer = nullptr;

	ZeroMemory(myIndexBufferDesc, sizeof(*myIndexBufferDesc));
	myIndexBufferDesc->Usage = D3D11_USAGE_IMMUTABLE;
	myIndexBufferDesc->BindFlags = D3D11_BIND_INDEX_BUFFER;
	myIndexBufferDesc->CPUAccessFlags = 0;
	myIndexBufferDesc->MiscFlags = 0;
	myIndexBufferDesc->StructureByteStride = 0;
}

void CFont::CreateConstantBuffer()
{
	myConstantStruct = new SFontConstantBuffer;

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(SFontConstantBuffer);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
	Engine::GetAPI()->SetDebugName(myConstantBuffer, "Font Constant Buffer");
	Engine::GetAPI()->HandleErrors(hr, "[Font] : Failed to Create Constant Buffer, ");
}

void CFont::UpdateBuffer()
{
	myColor = myDefaultColor;
	SAFE_RELEASE(myVertexBuffer->myVertexBuffer);
	SAFE_RELEASE(myIndexBuffer->myIndexBuffer);

	u32 count = u32(myText.length());
	float z = 0.f;
	float drawX = 0.f;
	float drawY = -5.f;
	float maxDrawY = 0.f;
	myVertices.RemoveAll();
	myIndices.RemoveAll();

	SVertexTypePosColUv v;
	u32 skips = 0;
	for (u32 i = 0, row = 0; i < count; i++)
	{
		SCharData& charData = myData->myCharData[myText[i]];
		
		if (maxDrawY < charData.myHeight)
		{
			maxDrawY = charData.myHeight;
		}

		if (myText[i] == '\n')
		{
			drawX = 0;
			drawY -= (maxDrawY + 6);
			row++;
			continue;
		}

		if (myText[i] == '#')
		{
			skips = 0;
			if (myText[i + 2] != 'x')
			{
				std::string hex_code = "0x" + myText.substr(i + 1, 6) + "FF";
				unsigned int _color = (unsigned int)strtoul(hex_code.c_str(), nullptr, 16);
				myColor.Convert(_color);
				i+=7;
				skips = 8;
				if (myText[i] != '(')
					myColor = myDefaultColor;
				continue;
			}
			else
			{ 
				std::string hex_code = myText.substr(i + 3, 6) + "FF"; //might extend this with a lerp alpha in the future?
				unsigned int _color = (unsigned int)strtoul(hex_code.c_str(), nullptr, 16);
				myColor.Convert(_color);
				i += 9;
				skips = 10;
				continue;
			}

			
		}

		if (myText[i] == ')') 
		{
			myColor = myDefaultColor;
			continue;
		}
			

		float left = drawX;
		float right = left + charData.myWidth;
		float top = drawY + charData.myBearingY;
		float bottom = top + charData.myHeight;

		v.myPosition = { left, bottom, z };
		v.myColor = myColor.ToVec4(); //{ float(myColor.m_re / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
		v.myUV = charData.myTopLeftUV;
		myVertices.Add(v);

		v.myPosition = { left, top, z };
		v.myColor = myColor.ToVec4();// { float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
		v.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
		myVertices.Add(v);

		v.myPosition = { right, bottom, z };
		v.myColor = myColor.ToVec4(); //{ float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
		v.myUV = { charData.myBottomRightUV.x, charData.myTopLeftUV.y };
		myVertices.Add(v);

		v.myPosition = { right, top, z };
		v.myColor = myColor.ToVec4(); // { float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
		v.myUV = charData.myBottomRightUV;
		myVertices.Add(v);

		u32 startIndex = (i - skips - row) * 4.f;

		myIndices.Add(startIndex + 1);
		myIndices.Add(startIndex + 0);
		myIndices.Add(startIndex + 2);

		myIndices.Add(startIndex + 2);
		myIndices.Add(startIndex + 3);
		myIndices.Add(startIndex + 1);

		drawX += charData.myBearingX;


	}

	if (myVertices.Size() <= 0)
		return;

	myVertexBufferDesc->ByteWidth = sizeof(SVertexTypePosColUv) * myVertices.Size();
	myInitData->pSysMem = reinterpret_cast<char*>(&myVertices[0]);
	HRESULT hr = Engine::GetAPI()->GetDevice()->CreateBuffer(myVertexBufferDesc, myInitData, &myVertexBuffer->myVertexBuffer);

	Engine::GetAPI()->SetDebugName(myVertexBuffer->myVertexBuffer, "Font Vertex Buffer");

	myIndexBufferDesc->ByteWidth = sizeof(UINT) * myIndices.Size();
	myInitData->pSysMem = reinterpret_cast<char*>(&myIndices[0]);
	hr = Engine::GetAPI()->GetDevice()->CreateBuffer(myIndexBufferDesc, myInitData, &myIndexBuffer->myIndexBuffer);

	Engine::GetAPI()->SetDebugName(myIndexBuffer->myIndexBuffer, "Font Index Buffer");

}

void CFont::UpdateConstantBuffer()
{
	DL_ASSERT_EXP(myConstantStruct != nullptr, "Vertex Constant Buffer Struct was null.");

	D3D11_MAPPED_SUBRESOURCE msr;
	Engine::GetAPI()->GetContext()->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (msr.pData != nullptr)
	{
		SFontConstantBuffer* ptr = (SFontConstantBuffer*)msr.pData;
		memcpy(ptr, &myConstantStruct->world.myMatrix[0], sizeof(SFontConstantBuffer));
	}

	Engine::GetAPI()->GetContext()->Unmap(myConstantBuffer, 0);
}


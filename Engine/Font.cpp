#include "stdafx.h"
#include "Font.h"
#include "FontManager.h"
#include <d3dx11effect.h>
#include "EffectContainer.h"
#include "Effect.h"
#include <d3d11.h>
#include "DirectX11.h"
#include "Engine.h"
#include "EngineDefines.h"
#include "VertexWrapper.h"
#include "IndexWrapper.h"
#include <TimeManager.h>
namespace Snowblind
{
	CFont::CFont(SFontData* aFontData)
	{
		myData = aFontData;
		myText = "";
		myEffect = CEffectContainer::GetInstance()->GetEffect("Data/Shaders/Font_Effect.fx");

		myVertexBufferDesc = new D3D11_BUFFER_DESC();
		myIndexBufferDesc = new D3D11_BUFFER_DESC();
		myInitData = new D3D11_SUBRESOURCE_DATA();


		CreateInputLayout();
		CreateVertexBuffer();
		CreateIndexBuffer();
		myDefaultColor.r = 255;
		myDefaultColor.g = 255;
		myDefaultColor.b = 255;
		myDefaultColor.a = 255;
		myColor = myDefaultColor;
		myPreviousAdvance = 0;

		myUpdateTimer = CU::TimeManager::GetInstance()->CreateTimer();
		myRenderTimer = CU::TimeManager::GetInstance()->CreateTimer();

	}

	CFont::~CFont()
	{
		SAFE_RELEASE(myVertexBuffer->myVertexBuffer);
		SAFE_RELEASE(myIndexBuffer->myIndexBuffer);
		SAFE_DELETE(myIndexBuffer);
		SAFE_DELETE(myVertexBuffer);


		SAFE_DELETE(myVertexBufferDesc);
		SAFE_DELETE(myIndexBufferDesc);
		SAFE_DELETE(myInitData);
		SAFE_RELEASE(myVertexLayout);
		SAFE_DELETE(myData);
	}

	void CFont::SetText(const std::string& aText)
	{
		CU::TimeManager::GetInstance()->GetTimer(myUpdateTimer).Update();
		if (myText != aText)
		{
			myText = aText;
			UpdateBuffer();
		}
		myUpdateTime = CU::TimeManager::GetInstance()->GetTimer(myUpdateTimer).GetFrameTime().GetMilliseconds();
	}

	const std::string& CFont::GetText() const
	{
		return myText;
	}

	void CFont::Render()
	{
		CU::TimeManager::GetInstance()->GetTimer(myRenderTimer).Update();
		if (!myEffect)
			return;
		myEffect->SetAlbedo(myData->myAtlasView);
		ID3D11DeviceContext& context = *CEngine::GetDirectX()->GetContext();
		context.IASetInputLayout(myVertexLayout);
		context.IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNrOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		context.IASetIndexBuffer(myIndexBuffer->myIndexBuffer, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			HRESULT hr = myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, &context);
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to apply pass to context!");
			context.DrawIndexed(myIndices.Size(), 0, 0);
		}

		myRenderTime = CU::TimeManager::GetInstance()->GetTimer(myRenderTimer).GetFrameTime().GetMilliseconds();
	}

	Snowblind::CEffect* CFont::GetEffect()
	{
		return myEffect;
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
		return myUpdateTime * 1000.f;
	}

	float CFont::GetRenderTime()
	{
		return myRenderTime * 1000.f;
	}

	void CFont::CreateInputLayout()
	{
		myVertexFormat.Init(3);
		myVertexFormat.Add(VertexLayoutPosColUV[0]);
		myVertexFormat.Add(VertexLayoutPosColUV[1]);
		myVertexFormat.Add(VertexLayoutPosColUV[2]);

		D3DX11_PASS_DESC passDesc;
		myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateInputLayout(&myVertexFormat[0], myVertexFormat.Size(), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
		CEngine::GetDirectX()->HandleErrors(hr, " [Font] : Input Layout.");
		CEngine::GetDirectX()->SetDebugName(myVertexLayout, "Font Input Layout");
	}

	void CFont::CreateVertexBuffer()
	{
		myVertexBuffer = new SVertexBufferWrapper;
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
		myIndexBuffer = new SIndexBufferWrapper;
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

	void CFont::UpdateBuffer()
	{
		SAFE_RELEASE(myVertexBuffer->myVertexBuffer);
		SAFE_RELEASE(myIndexBuffer->myIndexBuffer);

		int count = myText.length();
		float drawX = 0.f;
		float drawY = 0.f;
		float maxDrawY = 0.f;
		myVertices.RemoveAll();
		myIndices.RemoveAll();

		SVertexTypePosColUv v;
		for (int i = 0, iAsItShouldBe = 0, row = 0; i < count; i++, iAsItShouldBe++)
		{
			SCharData& charData = myData->myCharData[myText[i]];

			if (myText[i] == '}')
			{
				myColor = myDefaultColor;
				iAsItShouldBe--;
				continue;
			}

			if (myText[i] == '{')
			{
				iAsItShouldBe--;
				continue;
			}

			if (myText[i] == '#')
			{
				std::string color;
				int skip = i;
				for (int j = i + 1; j < i + 7; ++j)
				{
					color += myText[j];
					skip++;
				}
				myColor.r = std::strtoul(color.substr(0, 2).c_str(), NULL, 16);
				myColor.g = std::strtoul(color.substr(2, 2).c_str(), NULL, 16);
				myColor.b = std::strtoul(color.substr(4, 2).c_str(), NULL, 16);

				i = skip;
				iAsItShouldBe--;
				continue;
			}


			if (maxDrawY < charData.myHeight)
			{
				maxDrawY = charData.myHeight;
			}


			if (myText[i] == '\n')
			{
				drawX = 0;
				drawY -= (maxDrawY + 4);
				row++;
				continue;
			}


			if (myText[i] == ' ')
			{
				drawX += myData->myWordSpacing;
			}

			float left = drawX;
			float right = left + charData.myWidth;
			float top = drawY + charData.myBearingY;
			float bottom = top + charData.myHeight;



			v.myPosition = { left, bottom, 0 };
			v.myColor = { float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
			v.myUV = charData.myTopLeftUV;
			myVertices.Add(v);

			v.myPosition = { left, top, 0 };
			v.myColor = { float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
			v.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
			myVertices.Add(v);

			v.myPosition = { right, bottom, 0 };
			v.myColor = { float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
			v.myUV = { charData.myBottomRightUV.x, charData.myTopLeftUV.y };
			myVertices.Add(v);

			v.myPosition = { right, top, 0 };
			v.myColor = { float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
			v.myUV = charData.myBottomRightUV;
			myVertices.Add(v);

			int startIndex = (iAsItShouldBe - row) * 4.f;

			myIndices.Add(startIndex + 1);
			myIndices.Add(startIndex + 0);
			myIndices.Add(startIndex + 2);

			myIndices.Add(startIndex + 2);
			myIndices.Add(startIndex + 3);
			myIndices.Add(startIndex + 1);

			myPreviousAdvance = drawX;
			drawX += charData.myAdvanceX;
		}

		myVertexBufferDesc->ByteWidth = sizeof(SVertexTypePosColUv) * myVertices.Size();
		myInitData->pSysMem = reinterpret_cast<char*>(&myVertices[0]);
		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(myVertexBufferDesc, myInitData, &myVertexBuffer->myVertexBuffer);

		CEngine::GetDirectX()->SetDebugName(myVertexBuffer->myVertexBuffer, "Font Vertex Buffer");

		myIndexBufferDesc->ByteWidth = sizeof(UINT) * myIndices.Size();
		myInitData->pSysMem = reinterpret_cast<char*>(&myIndices[0]);
		hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(myIndexBufferDesc, myInitData, &myIndexBuffer->myIndexBuffer);

		CEngine::GetDirectX()->SetDebugName(myIndexBuffer->myIndexBuffer, "Font Index Buffer");

		//mySize.x = drawX;
		//mySize.y = drawY;
	}
};
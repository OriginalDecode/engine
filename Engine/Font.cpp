#include "stdafx.h"
#include "Font.h"
#include "FontManager.h"

#include "VertexWrapper.h"
#include "IndexWrapper.h"

#include <TimeManager.h>
#include "AssetsContainer.h"

namespace Snowblind
{
	CFont::CFont(SFontData* aFontData)
	{
		myData = aFontData;
		myText = " ";
		myEffect = CAssetsContainer::GetInstance()->GetEffect("Data/Shaders/Font_Effect.fx");

		myVertexBufferDesc = new D3D11_BUFFER_DESC();
		myIndexBufferDesc = new D3D11_BUFFER_DESC();
		myInitData = new D3D11_SUBRESOURCE_DATA();


		CreateInputLayout();
		CreateVertexBuffer();
		CreateIndexBuffer();
		InitiateBlendstate();

		myColor = myDefaultColor;

		myTimeManager = new CU::TimeManager();
		myUpdateTimer = myTimeManager->CreateTimer();
		myRenderTimer = myTimeManager->CreateTimer();


		myRenderTime = 0.f;
		myUpdateTime = 0.f;

	}

	CFont::~CFont()
	{
		SAFE_DELETE(myIndexBuffer);
		SAFE_DELETE(myVertexBuffer);
		SAFE_RELEASE(myBlendState);

		SAFE_DELETE(myVertexBufferDesc);
		SAFE_DELETE(myIndexBufferDesc);
		SAFE_DELETE(myTimeManager);

		SAFE_DELETE(myInitData);
		SAFE_RELEASE(myVertexLayout);
	}

	void CFont::SetText(const std::string& aText)
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

		if (!myEffect)
			return;
		myEffect->SetTexture(myData->myAtlasView, "FontTexture");
		myEffect->SetTexture(myData->myOutlineAtlasView, "OutlineTexture");

		ID3D11DeviceContext& context = *CEngine::GetDirectX()->GetContext();
		context.IASetInputLayout(myVertexLayout);
		context.IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNrOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		context.IASetIndexBuffer(myIndexBuffer->myIndexBuffer, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		float blendFactor[4];
		blendFactor[0] = 0.f;
		blendFactor[1] = 0.f;
		blendFactor[2] = 0.f;
		blendFactor[3] = 0.f;
		myEffect->SetBlendState(myBlendState, blendFactor);
		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			HRESULT hr = myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, &context);
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to apply pass to context!");
			context.DrawIndexed(myIndices.Size(), 0, 0);

		}

		myTimeManager->GetTimer(myRenderTimer).Update();
		myRenderTime = myTimeManager->GetTimer(myRenderTimer).GetTotalTime().GetMilliseconds() - myRenderTime;

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
		return myUpdateTime;
	}

	float CFont::GetRenderTime()
	{
		return myRenderTime;
	}

	void CFont::CreateInputLayout()
	{
		myVertexFormat.Init(4);
		myVertexFormat.Add(VertexLayoutPosColUV2[0]);
		myVertexFormat.Add(VertexLayoutPosColUV2[1]);
		myVertexFormat.Add(VertexLayoutPosColUV2[2]);
		myVertexFormat.Add(VertexLayoutPosColUV2[3]);


		D3DX11_PASS_DESC passDesc;
		myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateInputLayout(&myVertexFormat[0], myVertexFormat.Size(), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
		CEngine::GetDirectX()->HandleErrors(hr, " [Font] : Input Layout.");
		CEngine::GetDirectX()->SetDebugName(myVertexLayout, "Font Input Layout");
	}

	void CFont::CreateVertexBuffer()
	{
		myVertexBuffer = new SVertexBufferWrapper;
		myVertexBuffer->myStride = sizeof(SVertexTypePosColUv2);
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

	void CFont::InitiateBlendstate()
	{
		D3D11_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = true;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBlendState(&blendDesc, &myBlendState);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to create blendstate.");
		CEngine::GetDirectX()->SetDebugName(myBlendState, "Font : BlendState");
	}

	void CFont::UpdateBuffer()
	{
		SAFE_RELEASE(myVertexBuffer->myVertexBuffer);
		SAFE_RELEASE(myIndexBuffer->myIndexBuffer);

		int count = myText.length();
		float z = 0.f;
		float drawX = 0.f;
		float drawY = 0.f;
		float maxDrawY = 0.f;
		myVertices.RemoveAll();
		myIndices.RemoveAll();

		SVertexTypePosColUv2 v;
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
				if (myText.size() - 1 == i)
				{
					break;
				}
				std::string color;
				int skip = i;
				bool hasBreaked = false;
				for (int j = i + 1; j < i + 7; ++j)
				{
					if (myText[j] == 'I')
					{
						hasBreaked = true;
						break;
					}
					color += myText[j];
					skip++;
				}
				if (hasBreaked == false)
				{
					myColor.r = std::strtoul(color.substr(0, 2).c_str(), NULL, 16);
					myColor.g = std::strtoul(color.substr(2, 2).c_str(), NULL, 16);
					myColor.b = std::strtoul(color.substr(4, 2).c_str(), NULL, 16);
				}
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
			
			v.myPosition = { left, bottom, z };
			v.myColor = { float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
			v.myUV = charData.myTopLeftUV;
			v.myUV2 = charData.myTopLeftUVBorder;
			myVertices.Add(v);

			v.myPosition = { left, top, z };
			v.myColor = { float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
			v.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
			v.myUV2 = { charData.myTopLeftUVBorder.x, charData.myBottomRightUVBorder.y };
			myVertices.Add(v);

			v.myPosition = { right, bottom, z };
			v.myColor = { float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
			v.myUV = { charData.myBottomRightUV.x, charData.myTopLeftUV.y };
			v.myUV2 = { charData.myBottomRightUVBorder.x, charData.myTopLeftUVBorder.y };
			myVertices.Add(v);

			v.myPosition = { right, top, z };
			v.myColor = { float(myColor.r / 255.f), float(myColor.g / 255.f), float(myColor.b / 255.f), 1.f };
			v.myUV = charData.myBottomRightUV;
			v.myUV2 = charData.myBottomRightUVBorder;
			myVertices.Add(v);

			int startIndex = (iAsItShouldBe - row) * 4.f;

			myIndices.Add(startIndex + 1);
			myIndices.Add(startIndex + 0);
			myIndices.Add(startIndex + 2);

			myIndices.Add(startIndex + 2);
			myIndices.Add(startIndex + 3);
			myIndices.Add(startIndex + 1);

			drawX += charData.myBearingX;
		}

		myVertexBufferDesc->ByteWidth = sizeof(SVertexTypePosColUv2) * myVertices.Size();
		myInitData->pSysMem = reinterpret_cast<char*>(&myVertices[0]);
		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(myVertexBufferDesc, myInitData, &myVertexBuffer->myVertexBuffer);

		CEngine::GetDirectX()->SetDebugName(myVertexBuffer->myVertexBuffer, "Font Vertex Buffer");

		myIndexBufferDesc->ByteWidth = sizeof(UINT) * myIndices.Size();
		myInitData->pSysMem = reinterpret_cast<char*>(&myIndices[0]);
		hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(myIndexBufferDesc, myInitData, &myIndexBuffer->myIndexBuffer);

		CEngine::GetDirectX()->SetDebugName(myIndexBuffer->myIndexBuffer, "Font Index Buffer");

	}
};
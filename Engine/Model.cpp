#include "stdafx.h"
#include "Model.h"

#include "Camera.h"
#include "DirectX11.h"
#include <DL_Debug.h>
#include "Effect.h"
#include "Engine.h"
#include "EngineDefines.h"
#include "IndexWrapper.h"
#include "VertexWrapper.h"
#include "VertexTypes.h"
#include "Surface.h"
namespace Snowblind
{
	CModel::CModel()
	{
		myAPI = CEngine::GetDirectX();
	}

	CModel::~CModel()
	{
		myCamera = nullptr;
		mySurfaces.DeleteAll();
		myChildren.DeleteAll();

		SAFE_DELETE(myVertexBuffer);
		SAFE_DELETE(myVertexData);
		SAFE_DELETE(myIndexBuffer);
		SAFE_DELETE(myIndexData);
		SAFE_RELEASE(myVertexLayout);
		SAFE_DELETE(myBaseStruct);
		SAFE_RELEASE(myConstantBuffer);
	}

	void CModel::CreateTriangle(const std::string& anEffectPath)
	{
		myEffect = CAssetsContainer::GetInstance()->GetEffect("Data/Shaders/T_Line3D.json");
		myIsNULLObject = false;
		//myEffect = CAssetsContainer::GetInstance()->GetEffect(anEffectPath);
		myVertexFormat.Init(2);
		myVertexFormat.Add(VertexLayoutPosCol[0]);
		myVertexFormat.Add(VertexLayoutPosCol[1]);
		SVertexTypePosCol v;
		v.myPosition = { 0.f, 0.5f, 0.f };
		v.myColor = { 1.f, 0.f, 0.f, 1.f };
		myVertices.Add(v);

		v.myPosition = { 0.5f, -0.5f, 0.f };
		v.myColor = { 0.f, 0.f, 1.f, 1.f };
		myVertices.Add(v);

		v.myPosition = { -0.5f, -0.5f, 0.f };
		v.myColor = { 0.f, 1.f, 0.f, 1.f };
		myVertices.Add(v);

		myVertexData = new SVertexDataWrapper;

		myVertexData->myNrOfVertexes = myVertices.Size();
		myVertexData->myStride = sizeof(SVertexTypePosCol);
		myVertexData->mySize = myVertexData->myNrOfVertexes * myVertexData->myStride;
		myVertexData->myVertexData = new char[myVertexData->mySize]();
		memcpy(myVertexData->myVertexData, &myVertices[0], myVertexData->mySize);


		InitVertexBuffer();
		InitConstantBuffer();
	}

	void CModel::CreateCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth)
	{
		myEffect = CAssetsContainer::GetInstance()->GetEffect(anEffectPath);
		myIsNULLObject = false;
		ENGINE_LOG("Creating Cube");
		float halfWidth = aWidth *0.5f;
		float halfDepth = aDepth *0.5f;
		float halfHeight = aHeight *0.5f;

		CU::GrowingArray<SVertexTypePosCol> vertices;
		CU::GrowingArray<int> indexes;

		myVertexFormat.Init(2);
		myVertexFormat.Add(VertexLayoutPosCol[0]);
		myVertexFormat.Add(VertexLayoutPosCol[1]);

		//myEffect = CAssetsContainer::GetInstance()->GetEffect(anEffectPath);

		SVertexTypePosCol tempVertex;

#pragma region Vertex
		tempVertex.myPosition = { -halfWidth, halfHeight, -halfDepth };
		tempVertex.myColor = { 0.0f, 0.0f, 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, halfHeight, -halfDepth };
		tempVertex.myColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, halfHeight, halfDepth };
		tempVertex.myColor = { 0.0f, 1.0f, 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -halfWidth, halfHeight, halfDepth };
		tempVertex.myColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -halfWidth, -halfHeight, -halfDepth };
		tempVertex.myColor = { 1.0f, 0.0f, 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, -halfHeight, -halfDepth };
		tempVertex.myColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { halfWidth, -halfHeight, halfDepth };
		tempVertex.myColor = { 0.0f, 1.0f, 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -halfWidth, -halfHeight, halfDepth };
		tempVertex.myColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		vertices.Add(tempVertex);
#pragma endregion

#pragma region Index

		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(0);

		indexes.Add(3);
		indexes.Add(2);
		indexes.Add(1);

		indexes.Add(0);
		indexes.Add(5);
		indexes.Add(4);

		indexes.Add(1);
		indexes.Add(5);
		indexes.Add(0);

		indexes.Add(3);
		indexes.Add(4);
		indexes.Add(7);
		indexes.Add(0);
		indexes.Add(4);
		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(6);
		indexes.Add(5);
		indexes.Add(2);
		indexes.Add(6);
		indexes.Add(1);
		indexes.Add(2);
		indexes.Add(7);
		indexes.Add(6);
		indexes.Add(3);
		indexes.Add(7);
		indexes.Add(2);
		indexes.Add(6);
		indexes.Add(4);
		indexes.Add(5);
		indexes.Add(7);
		indexes.Add(4);
		indexes.Add(6);
#pragma endregion

		myVertexData = new SVertexDataWrapper;
		myIndexData = new SVertexIndexWrapper;

		myVertexData->myNrOfVertexes = vertices.Size();
		myVertexData->myStride = sizeof(SVertexTypePosCol);
		myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
		myVertexData->myVertexData = new char[myVertexData->mySize]();
		memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);

		myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
		myIndexData->myIndexCount = 6 * 6;
		myIndexData->mySize = myIndexData->myIndexCount * 4;

		myIndexData->myIndexData = new char[myIndexData->mySize];
		memcpy(myIndexData->myIndexData, &indexes[0], myIndexData->mySize);

		InitVertexBuffer();
		InitIndexBuffer();
	}

	void CModel::CreateCube(const std::string& anEffectPath, const CU::Vector3f& aColor)
	{
		myEffect = CAssetsContainer::GetInstance()->GetEffect(anEffectPath);

		myIsNULLObject = false;
		ENGINE_LOG("Creating Cube");

		CU::GrowingArray<SVertexTypePosCol> vertices;
		CU::GrowingArray<int> indexes;

		myVertexFormat.Init(2);
		myVertexFormat.Add(VertexLayoutPosCol[0]);
		myVertexFormat.Add(VertexLayoutPosCol[1]);

		float size = 0.01f;
		SVertexTypePosCol tempVertex;
		CU::Vector4f color = aColor;
		color.a = 1.f;
#pragma region Vertex
		tempVertex.myPosition = { -size, size, -size };
		tempVertex.myColor = color;
		vertices.Add(tempVertex);

		tempVertex.myPosition = { size, size, -size };
		tempVertex.myColor = color;
		vertices.Add(tempVertex);

		tempVertex.myPosition = { size, size, size };
		tempVertex.myColor = color;
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -size, size, size };
		tempVertex.myColor = color;
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -size, -size, -size };
		tempVertex.myColor = color;
		vertices.Add(tempVertex);

		tempVertex.myPosition = { size, -size, -size };
		tempVertex.myColor = color;
		vertices.Add(tempVertex);

		tempVertex.myPosition = { size, -size, size };
		tempVertex.myColor = color;
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -size, -size, size };
		tempVertex.myColor = color;
		vertices.Add(tempVertex);
#pragma endregion

#pragma region Index

		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(0);

		indexes.Add(3);
		indexes.Add(2);
		indexes.Add(1);

		indexes.Add(0);
		indexes.Add(5);
		indexes.Add(4);

		indexes.Add(1);
		indexes.Add(5);
		indexes.Add(0);

		indexes.Add(3);
		indexes.Add(4);
		indexes.Add(7);
		indexes.Add(0);
		indexes.Add(4);
		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(6);
		indexes.Add(5);
		indexes.Add(2);
		indexes.Add(6);
		indexes.Add(1);
		indexes.Add(2);
		indexes.Add(7);
		indexes.Add(6);
		indexes.Add(3);
		indexes.Add(7);
		indexes.Add(2);
		indexes.Add(6);
		indexes.Add(4);
		indexes.Add(5);
		indexes.Add(7);
		indexes.Add(4);
		indexes.Add(6);
#pragma endregion

		myVertexData = new SVertexDataWrapper;
		myIndexData = new SVertexIndexWrapper;

		myVertexData->myNrOfVertexes = vertices.Size();
		myVertexData->myStride = sizeof(SVertexTypePosCol);
		myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
		myVertexData->myVertexData = new char[myVertexData->mySize]();
		memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);

		myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
		myIndexData->myIndexCount = 6 * 6;
		myIndexData->mySize = myIndexData->myIndexCount * 4;

		myIndexData->myIndexData = new char[myIndexData->mySize];
		memcpy(myIndexData->myIndexData, &indexes[0], myIndexData->mySize);

		InitVertexBuffer();
		InitIndexBuffer();
		InitConstantBuffer();
	}

	void CModel::CreateTexturedCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth)
	{
		anEffectPath;
		myIsNULLObject = false;
		myIsTextured = true;
		//float halfWidth = aWidth * 0.5f;
		//float halfDepth = aDepth * 0.5f;
		//float halfHeight = aHeight * 0.5f;
		aWidth;
		aDepth;
		aHeight;

		CU::GrowingArray<SVertexTypePosNormUV> vertices;
		CU::GrowingArray<int> indexes;

		myVertexFormat.Init(3);
		myVertexFormat.Add(VertexLayoutPosNormUV[0]);
		myVertexFormat.Add(VertexLayoutPosNormUV[1]);
		myVertexFormat.Add(VertexLayoutPosNormUV[2]);


		//myEffect = CAssetsContainer::GetInstance()->GetEffect(anEffectPath);

#pragma region Vertex
		SVertexTypePosNormUV tempVertex;

		//Top
#pragma region Top
		tempVertex.myPosition = { -1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 1.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 1.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 1.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 1.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 1.0f };
		vertices.Add(tempVertex);
#pragma endregion
		//Bottom
#pragma region Bottom
		tempVertex.myPosition = { -1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, -1.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, -1.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, -1.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, -1.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 1.0f };
		vertices.Add(tempVertex);
#pragma endregion
		//Left
#pragma region Left
		tempVertex.myPosition = { -1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { -1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { -1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { -1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { -1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 1.0f };
		vertices.Add(tempVertex);
#pragma endregion
		//Right
#pragma region Right
		tempVertex.myPosition = { 1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { 1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { 1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { 1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { 1.0f, 0.0f, 0.0f };
		tempVertex.myUV = { 0.0f, 1.0f };
		vertices.Add(tempVertex);
#pragma endregion
		//Front
#pragma region Front
		tempVertex.myPosition = { -1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, -1.0f };
		tempVertex.myUV = { 0.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, -1.0f };
		tempVertex.myUV = { 1.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, -1.0f };
		tempVertex.myUV = { 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, -1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, -1.0f };
		tempVertex.myUV = { 0.0f, 1.0f };
		vertices.Add(tempVertex);
#pragma endregion
		//Back
#pragma region Back
		tempVertex.myPosition = { -1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, 1.0f };
		tempVertex.myUV = { 0.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, -1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, 1.0f };
		tempVertex.myUV = { 1.0f, 0.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { 1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, 1.0f };
		tempVertex.myUV = { 1.0f, 1.0f };
		vertices.Add(tempVertex);

		tempVertex.myPosition = { -1.0f, 1.0f, 1.0f };
		tempVertex.myNormal = { 0.0f, 0.0f, 1.0f };
		tempVertex.myUV = { 0.0f, 1.0f };
		vertices.Add(tempVertex);
#pragma endregion

#pragma endregion

#pragma region Index
		//Top
		indexes.Add(3);
		indexes.Add(1);
		indexes.Add(0);

		indexes.Add(2);
		indexes.Add(1);
		indexes.Add(3);

		//Bottom
		indexes.Add(6);
		indexes.Add(4);
		indexes.Add(5);

		indexes.Add(7);
		indexes.Add(4);
		indexes.Add(6);

		//Left
		indexes.Add(11);
		indexes.Add(9);
		indexes.Add(8);

		indexes.Add(10);
		indexes.Add(9);
		indexes.Add(11);

		//Right
		indexes.Add(14);
		indexes.Add(12);
		indexes.Add(13);

		indexes.Add(15);
		indexes.Add(12);
		indexes.Add(14);

		//Front
		indexes.Add(19);
		indexes.Add(17);
		indexes.Add(16);

		indexes.Add(18);
		indexes.Add(17);
		indexes.Add(19);

		//Back
		indexes.Add(22);
		indexes.Add(20);
		indexes.Add(21);

		indexes.Add(23);
		indexes.Add(20);
		indexes.Add(22);
#pragma endregion

		myVertexData = new SVertexDataWrapper;
		myIndexData = new SVertexIndexWrapper;
		myBaseStruct = new SVertexBaseStruct;
		myVertexData->myNrOfVertexes = vertices.Size();
		myVertexData->myStride = sizeof(SVertexTypePosNormUV);
		myVertexData->mySize = myVertexData->myNrOfVertexes*myVertexData->myStride;
		myVertexData->myVertexData = new  char[myVertexData->mySize]();
		memcpy(myVertexData->myVertexData, &vertices[0], myVertexData->mySize);

		myIndexData->myFormat = DXGI_FORMAT_R32_UINT;
		myIndexData->myIndexCount = indexes.Size();
		myIndexData->mySize = myIndexData->myIndexCount * 4;

		myIndexData->myIndexData = new char[myIndexData->mySize];
		memcpy(myIndexData->myIndexData, &indexes[0], myIndexData->mySize);

		/*	CSurface* tempSurface = new CSurface(myEffect);
			tempSurface->SetVertexStart(0);
			tempSurface->SetIndexCount(myIndexData->myIndexCount);
			tempSurface->SetPrimology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			tempSurface->SetTexture("AlbedoTexture", "Data/Textures/col.dds");
			mySurfaces.Add(tempSurface);*/

			//myEffect->SetAlbedo(Snowblind::CAssetsContainer::GetInstance()->GetTexture("Data/Textures/colors.dds"));
		InitVertexBuffer();
		InitIndexBuffer();
	}

	void CModel::CreateModel()
	{
		myIsTextured = true;
		if (myIsNULLObject == false)
		{
			InitVertexBuffer();
			InitIndexBuffer();
			InitConstantBuffer();
		}

		for each (CModel* child in myChildren)
		{
			child->CreateModel();
		}
	}

	void CModel::Render(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection)
	{
		if (!myIsNULLObject)
		{
			ID3D11DeviceContext* context = myAPI->GetContext();

			context->IASetInputLayout(myVertexLayout);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetVertexBuffers(0, 1, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
			context->IASetIndexBuffer(myIndexBuffer->myIndexBuffer, DXGI_FORMAT_R32_UINT, myIndexBuffer->myByteOffset);

			if (!myIsLightmesh)
			{
				CEngine::GetDirectX()->SetVertexShader(myEffect->GetVertexShader() ? myEffect->GetVertexShader()->vertexShader : nullptr);
				CEngine::GetDirectX()->SetPixelShader(myEffect->GetPixelShader() ? myEffect->GetPixelShader()->pixelShader : nullptr);
			}

			if (mySurfaces.Size() > 0)
			{

				if (!myIsSkysphere && !myIsLightmesh)
				{
					for (int i = 0; i < mySurfaces.Size(); i++)
					{
						SetMatrices(aCameraOrientation, aCameraProjection);
						context->VSSetConstantBuffers(0, 1, &myConstantBuffer);

						CEngine::GetDirectX()->SetSamplerState(eSamplerStates::LINEAR_WRAP);
						mySurfaces[i]->Activate();
						context->DrawIndexed(mySurfaces[i]->GetVertexCount(), 0, 0);
						/* Deactivate surface needed */
					}

				}
				else if (myIsSkysphere)
				{
					for (int i = 0; i < mySurfaces.Size(); i++)
					{
						CEngine::GetDirectX()->SetSamplerState(eSamplerStates::LINEAR_WRAP);

						SetMatrices(aCameraOrientation, aCameraProjection);

						context->VSSetConstantBuffers(0, 1, &myConstantBuffer);

						context->DrawIndexed(mySurfaces[i]->GetVertexCount(), 0, 0);

					}
				}
				else if (myIsLightmesh)
				{
					for (int i = 0; i < mySurfaces.Size(); i++)
					{
						CEngine::GetDirectX()->SetSamplerState(eSamplerStates::POINT_CLAMP);
						context->DrawIndexed(mySurfaces[i]->GetVertexCount(), 0, 0);
					}
				}
			}
			else
			{
				SetMatrices(aCameraOrientation, aCameraProjection);
				context->VSSetConstantBuffers(0, 1, &myConstantBuffer);
				context->DrawIndexed(myIndexData->myIndexCount, 0, 0);
			}
		}
		for each(CModel* child in myChildren)
		{
			child->SetPosition(myOrientation.GetPosition());
			child->Render(aCameraOrientation, aCameraProjection);
		}
	}

	void CModel::RenderPolygon(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection)
	{
		ID3D11DeviceContext* context = myAPI->GetContext();
		context->IASetInputLayout(myVertexLayout);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetVertexBuffers(0, 1, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myStride);

		CEngine::GetDirectX()->SetVertexShader(myEffect->GetVertexShader() ? myEffect->GetVertexShader()->vertexShader : nullptr);
		CEngine::GetDirectX()->SetPixelShader(myEffect->GetPixelShader() ? myEffect->GetPixelShader()->pixelShader : nullptr);

		SetMatrices(aCameraOrientation, aCameraProjection);
		context->VSSetConstantBuffers(0, 1, &myConstantBuffer);
		context->Draw(3, 0);
	}

	void CModel::SetIsSkysphere()
	{
		myIsSkysphere = true;
		for (int i = 0; i < myChildren.Size(); i++)
		{
			myChildren[i]->SetIsSkysphere();
		}
	}

	void CModel::SetIsLightmesh()
	{
		myIsLightmesh = true;
		for (int i = 0; i < myChildren.Size(); i++)
		{
			myChildren[i]->SetIsLightmesh();
		}
	}

	void CModel::SetPosition(const CU::Vector3f& aPosition)
	{
		myOrientation.SetPosition(aPosition);
		for each (CModel* child in myChildren)
		{
			child->SetPosition(aPosition);
		}
	}

	CU::Matrix44f& CModel::GetOrientation()
	{
		return myOrientation;
	}

	void CModel::SetEffect(CEffect* anEffect)
	{
		myEffect = anEffect;
	}

	void CModel::SetMatrices(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection)
	{
		if (!myIsNULLObject)
		{
			DL_ASSERT_EXP(myBaseStruct != nullptr, "Vertex Constant Buffer Struct was null.");

			myBaseStruct->world = myOrientation;
			myBaseStruct->invertedView = CU::Math::Inverse(aCameraOrientation);
			myBaseStruct->projection = aCameraProjection;


			D3D11_MAPPED_SUBRESOURCE msr;
			CEngine::GetDirectX()->GetContext()->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			if (msr.pData != nullptr)
			{
				SVertexBaseStruct* ptr = (SVertexBaseStruct*)msr.pData;
				memcpy(ptr, &myBaseStruct->world.myMatrix[0], sizeof(SVertexBaseStruct));
			}

			CEngine::GetDirectX()->GetContext()->Unmap(myConstantBuffer, 0);
		}
	}

	void CModel::InitVertexBuffer()
	{
		myVertexBuffer = new SVertexBufferWrapper();
		HRESULT hr;
		void* shader = myEffect->GetVertexShader()->compiledShader;
		int size = myEffect->GetVertexShader()->shaderSize;

		hr = myAPI->GetDevice()->
			CreateInputLayout(&myVertexFormat[0], myVertexFormat.Size()
				, shader
				, size
				, &myVertexLayout);

		CEngine::GetDirectX()->SetDebugName(myVertexLayout, "Model Vertex Layout");
		myAPI->HandleErrors(hr, "Failed to create VertexLayout");

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = myVertexData->mySize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = static_cast<void*>(myVertexData->myVertexData);

		hr = myAPI->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer->myVertexBuffer);
		myAPI->SetDebugName(myVertexBuffer->myVertexBuffer, "Model : Vertex Buffer");
		myAPI->HandleErrors(hr, "Failed to Create VertexBuffer!");

		myVertexBuffer->myStride = myVertexData->myStride;
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNrOfBuffers = 1;

	}

	void CModel::InitIndexBuffer()
	{
		myIndexBuffer = new SIndexBufferWrapper;

		D3D11_BUFFER_DESC indexDesc;
		ZeroMemory(&indexDesc, sizeof(indexDesc));
		indexDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexDesc.ByteWidth = myIndexData->mySize;
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;
		indexDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		ZeroMemory(&indexData, sizeof(indexData)),
			indexData.pSysMem = myIndexData->myIndexData;

		HRESULT hr = myAPI->GetDevice()->CreateBuffer(&indexDesc, &indexData, &myIndexBuffer->myIndexBuffer);
		myAPI->SetDebugName(myIndexBuffer->myIndexBuffer, "Model : Index Buffer");
		myAPI->HandleErrors(hr, "Failed to Create IndexBuffer");

		myIndexBuffer->myIndexBufferFormat = myIndexData->myFormat;
		myIndexBuffer->myByteOffset = 0;

	}

	void CModel::InitConstantBuffer()
	{
		myBaseStruct = new SVertexBaseStruct;

		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SVertexBaseStruct);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
		CEngine::GetDirectX()->SetDebugName(myConstantBuffer, "Model Constant Buffer");
		CEngine::GetDirectX()->HandleErrors(hr, "[Model] : Failed to Create Constant Buffer, ");
	}

	void CModel::AddChild(CModel* aChild)
	{
		myChildren.Add(aChild);
	}

}
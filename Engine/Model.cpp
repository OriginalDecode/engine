#include "stdafx.h"
#include "Model.h"

#include "Camera.h"
#include "DirectX11.h"
#include <DL_Debug.h>
#include "Engine.h"
#include "Surface.h"

namespace Snowblind
{
	CModel::CModel()
	{
		myModelStates.reset();
	}

	CModel::~CModel()
	{
	
		
	}

	bool CModel::CleanUp()
	{
		SAFE_DELETE(myVertexBuffer);
		if (myVertexBuffer)
			return false;
		SAFE_DELETE(myIndexBuffer);
		if (myIndexBuffer)
			return false;
		SAFE_DELETE(myVertexData);
		if (myVertexData)
			return false;
		SAFE_DELETE(myIndexData);
		if (myIndexData)
			return false;
		SAFE_DELETE(myConstantStruct);
		if (myConstantStruct)
			return false;
		SAFE_RELEASE(myConstantBuffer);
		if (myConstantBuffer)
			return false;
		SAFE_RELEASE(myVertexLayout);
		if (myVertexLayout)
			return false;

		mySurfaces.DeleteAll();
		myChildren.DeleteAll();
		return true;
	}

	CModel* CModel::CreateModel()
	{
		
		BeginTicketMutex(&g_ModelMutex);
		if (myIsNULLObject == false)
		{
			InitVertexBuffer();
			InitIndexBuffer();
			InitConstantBuffer();
		}
		EndTicketMutex(&g_ModelMutex);

		for each (CModel* child in myChildren)
		{
			child->CreateModel();
		}

		return this;
	}

	void CModel::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
	{
#ifdef SNOWBLIND_DX11
		if (!myIsNULLObject)
		{
			__super::Render(aCameraOrientation, aCameraProjection);
			if (!myIsLightMesh)
				myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			if (mySurfaces.Size() > 0)
			{
				//Check the lightpass if you're confused about when the lightmeshes gets their constantbuffers.

				for (u32 i = 0; i < mySurfaces.Size(); i++)
				{
					myAPI->SetSamplerState(eSamplerStates::LINEAR_WRAP);
					mySurfaces[i]->Activate();
					if (!myIsLightMesh)
						myContext->DrawIndexed(mySurfaces[i]->GetIndexCount(), 0, 0);
					else
						myContext->Draw(mySurfaces[i]->GetVertexCount(), 0);

					mySurfaces[i]->Deactivate();
				}

			}
		}
		for (CModel* child : myChildren)
		{
			child->SetPosition(myOrientation.GetPosition());
			child->Render(aCameraOrientation, aCameraProjection);
		}
#endif
	}

	void CModel::SetIsLightmesh()
	{
		if (this)
		{
			myIsLightMesh = true;
			for (int i = 0; i < myChildren.Size(); i++)
			{
				myChildren[i]->SetIsLightmesh();
			}
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

	void CModel::Update(float dt)
	{
		myOrientation = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(0.5f)* dt) * myOrientation;
		for (CModel* child : myChildren)
		{
			child->Update(dt);
		}
	}

	void CModel::SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
	{
#ifdef SNOWBLIND_DX11
		if (myIsNULLObject == false)
		{
			DL_ASSERT_EXP(myConstantStruct != nullptr, "Vertex Constant Buffer Struct was null.");

			myConstantStruct->world = myOrientation;
			myConstantStruct->invertedView = CU::Math::Inverse(aCameraOrientation);
			myConstantStruct->projection = aCameraProjection;


			D3D11_MAPPED_SUBRESOURCE msr;
			myAPI->GetContext()->Map(myConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			if (msr.pData != nullptr)
			{
				SVertexBaseStruct* ptr = (SVertexBaseStruct*)msr.pData;
				memcpy(ptr, &myConstantStruct->world.myMatrix[0], sizeof(SVertexBaseStruct));
			}

			myAPI->GetContext()->Unmap(myConstantBuffer, 0);
		}
#endif
	}

	void CModel::AddChild(CModel* aChild)
	{
		myChildren.Add(aChild);
	}

	void CModel::InitConstantBuffer()
	{
#ifdef SNOWBLIND_DX11
		if (!myConstantStruct)
			myConstantStruct = new SVertexBaseStruct;
		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(SVertexBaseStruct);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HRESULT hr = myAPI->GetDevice()->CreateBuffer(&cbDesc, 0, &myConstantBuffer);
		myAPI->SetDebugName(myConstantBuffer, "Model cb");
		myAPI->HandleErrors(hr, "[BaseModel] : Failed to Create Constant Buffer, ");
#endif
	}
};
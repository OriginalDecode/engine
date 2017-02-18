#include "stdafx.h"
#include "Model.h"

#include "Camera.h"
#include "DirectX11.h"
#include <DL_Debug.h>
#include "Engine.h"
#include "Surface.h"

namespace Hex
{
	CModel::CModel()
	{
		m_WHD = { 0.f,0.f,0.f };
		myModelStates.reset();
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

		for (CModel* children : myChildren)
		{
			children->CleanUp();
		}
		myChildren.DeleteAll();
		return true;
	}

	CModel* CModel::CreateModel(const std::string& filename)
	{
		m_Filename = CL::substr(filename, "/", false, 0);
		BeginTicketMutex(&g_ModelMutex);
		if (myIsNULLObject == false)
		{
			InitVertexBuffer();
			InitIndexBuffer();
			InitConstantBuffer();
		}
		EndTicketMutex(&g_ModelMutex);

		for (CModel* child : myChildren)
		{
			child->CreateModel(filename);
		}

		return this;
	}

	void CModel::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const CU::Vector4f& scale, bool render_shadows)
	{
#ifdef SNOWBLIND_DX11
		if (!myIsNULLObject)
		{
			__super::Render(aCameraOrientation, aCameraProjection, scale, render_shadows);

			if (!myIsLightMesh)
				myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);

			if (mySurfaces.Size() > 0)
			{
				for (s32 i = 0; i < mySurfaces.Size(); i++)
				{
					myAPI->SetSamplerState(eSamplerStates::LINEAR_WRAP);
					if (!myIsLightMesh)
					{
						if (!render_shadows && !myIsLightMesh)
							mySurfaces[i]->Activate(); //Gets a ton of junk data. What???

						myContext->DrawIndexed(mySurfaces[i]->GetIndexCount(), 0, 0);
					}
					else
					{
						myContext->Draw(mySurfaces[i]->GetVertexCount(), 0);
					}

					if (!render_shadows && !myIsLightMesh)
						mySurfaces[i]->Deactivate();
				}
			}
		}
		for (CModel* child : myChildren)
		{
			child->SetPosition(myOrientation.GetPosition());
			child->Render(aCameraOrientation, aCameraProjection, scale, render_shadows);
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

	void CModel::SetOrientation(CU::Matrix44f orientation)
	{
		myOrientation = orientation; 
		for (CModel* child : myChildren)
		{
			child->SetOrientation(myOrientation);
		}
	}

	void CModel::Update(float dt)
	{
		myOrientation = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(0.5f)* dt) * myOrientation;
		for (CModel* child : myChildren)
		{
			child->Update(dt);
		}
	}

	std::vector<float> CModel::GetVertices()
	{
		std::vector<float> to_return;
		for (const SVertexTypePosCol& vert : myVertices)
		{
			to_return.push_back(vert.myPosition.x);
			to_return.push_back(vert.myPosition.y);
			to_return.push_back(vert.myPosition.z);
		}
		return to_return;
	}

	std::vector<s32> CModel::GetIndices()
	{
		std::vector<s32> to_return;
		for (s32 index : m_Indices)
		{
			to_return.push_back(index);
		}
		return to_return;
	}

	void CModel::SetWHD(CU::Vector3f whd)
	{
		m_WHD = whd;
	}

	void CModel::UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const CU::Vector4f& scale)
	{
		if (myIsNULLObject == false)
		{
			DL_ASSERT_EXP(myConstantStruct != nullptr, "Vertex Constant Buffer Struct was null.");

			myConstantStruct->world = myOrientation;
			myConstantStruct->invertedView = CU::Math::Inverse(aCameraOrientation);
			myConstantStruct->projection = aCameraProjection;
			myConstantStruct->scale = scale;

			myAPI->UpdateConstantBuffer(myConstantBuffer, myConstantStruct);

		}
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

		myAPI->SetDebugName(myConstantBuffer, "Model Constant Buffer : " + m_Filename);
		myAPI->HandleErrors(hr, "[BaseModel] : Failed to Create Constant Buffer, ");
#endif
	}
};
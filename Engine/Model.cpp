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
		mySurfaces.DeleteAll();
		myChildren.DeleteAll();
	}

	CModel* CModel::CreateModel()
	{
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

		return this;
	}

	void CModel::Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
	{
		if (!myIsNULLObject)
		{
			__super::Render(aCameraOrientation, aCameraProjection);
			if (mySurfaces.Size() > 0)
			{
				//Check the lightpass if you're confused about when the lightmeshes gets their constantbuffers.
				if(!myIsLightMesh)
					myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
				for (int i = 0; i < mySurfaces.Size(); i++)
				{
					myAPI->SetSamplerState(eSamplerStates::LINEAR_WRAP);
					mySurfaces[i]->Activate();
					myContext->DrawIndexed(mySurfaces[i]->GetIndexCount(), 0, 0);
					mySurfaces[i]->Deactivate();
				}

			}
		}
		for each(CModel* child in myChildren)
		{
			child->SetPosition(myOrientation.GetPosition());
			child->Render(aCameraOrientation, aCameraProjection);
		}
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

	void CModel::SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection)
	{
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
	}

	void CModel::AddChild(CModel* aChild)
	{
		myChildren.Add(aChild);
	}
};
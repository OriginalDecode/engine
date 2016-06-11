#include "stdafx.h"
#include "FBXFactory.h"
#include "FbxLoader.h"

#include <DL_Debug.h>

#include "Model.h"
#include "Surface.h"
#include <Math/Matrix/Matrix44.h>
#include <TimeManager.h>

#include "VertexWrapper.h"
#include "IndexWrapper.h"
#include "AssetsContainer.h"
FBXFactory::FBXFactory()
{
	myLoader = new FBXLoader();
	myTimeManager = new CU::TimeManager();
	myLoadTimer = myTimeManager->CreateTimer();
}

FBXFactory::~FBXFactory()
{
	SAFE_DELETE(myTimeManager);
	SAFE_DELETE(myLoader);
}

void FBXFactory::FillData(ModelData* someData, Snowblind::CModel* someOutData, Snowblind::CEffect* anEffect)
{
	Snowblind::SVertexIndexWrapper* indexWrapper = new Snowblind::SVertexIndexWrapper();
	indexWrapper->myFormat = DXGI_FORMAT_R32_UINT;
	unsigned int* indexData = new unsigned int[someData->myIndexCount];
	memcpy(indexData, someData->myIndicies, someData->myIndexCount * sizeof(unsigned int));
	indexWrapper->myIndexData = (char*)indexData;
	indexWrapper->mySize = someData->myIndexCount * sizeof(unsigned int);
	someOutData->myIndexData = indexWrapper;

	Snowblind::SVertexDataWrapper* vertexData = new Snowblind::SVertexDataWrapper();
	someData->myVertexBuffer;
	int sizeOfBuffer = someData->myVertexCount*someData->myVertexStride * sizeof(float);
	char* vertexRawData = new char[sizeOfBuffer];
	memcpy(vertexRawData, someData->myVertexBuffer, sizeOfBuffer);
	vertexData->myVertexData = vertexRawData;
	vertexData->myNrOfVertexes = someData->myVertexCount;
	vertexData->mySize = sizeOfBuffer;
	vertexData->myStride = someData->myVertexStride * sizeof(float);

	//vertexData.myType = Ve;
	someOutData->myVertexData = vertexData;

	for (int i = 0; i < someData->myLayout.Size(); ++i)
	{
		auto currentLayout = someData->myLayout[i];
		D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC();
		desc->SemanticIndex = 0;
		desc->AlignedByteOffset = currentLayout.myOffset;
		desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc->InputSlot = 0;
		desc->InstanceDataStepRate = 0;

		if (currentLayout.myType == ModelData::VERTEX_POS)
		{
			desc->SemanticName = "POSITION";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_NORMAL)
		{
			desc->SemanticName = "NORMA";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_UV)
		{
			desc->SemanticName = "TEXCOORD";
			desc->Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BINORMAL)
		{
			desc->SemanticName = "BINORMA";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_TANGENT)
		{
			desc->SemanticName = "TANGENT";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_SKINWEIGHTS)
		{
			break;
			desc->SemanticName = "WEIGHTS";
			desc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BONEID)
		{
			break;
			desc->SemanticName = "BONES";
			desc->Format = DXGI_FORMAT_R32G32B32A32_SINT;
		}
		someOutData->myVertexFormat.Add(*desc);
	}
	someOutData->myIsNULLObject = false;

	Snowblind::CSurface* surface = new Snowblind::CSurface(0, someData->myVertexCount, 0, someData->myIndexCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	bool useSRGB = true;
	for (unsigned int i = 0; i < someData->myTextures.size(); ++i)
	{
		auto& currentTexture = someData->myTextures[i];

		std::string resourceName;
		if (currentTexture.myType == DIFFUSE)
		{
			resourceName = "AlbedoTexture";
		}
		if (currentTexture.myType == NORMALMAP)
		{
			resourceName = "NormalTexture";
			useSRGB = false;
		}
		if (currentTexture.myType == ROUGHNESS)
		{
			resourceName = "RoughnessTexture";
		}
		if (currentTexture.myType == SUBSTANCE)
		{
			resourceName = "MetalnessTexture";
		}
		if (currentTexture.myType == AO)
		{
			resourceName = "AOTexture";
		}

		std::string fileName(currentTexture.myFileName.begin(), currentTexture.myFileName.end());
		surface->SetEffect(anEffect);
		surface->SetTexture(resourceName, fileName);

	}
	someOutData->mySurfaces.Add(surface);
}

Snowblind::CModel* FBXFactory::CreateModel(FbxModelData* someModelData, Snowblind::CEffect* anEffect)
{
	Snowblind::CModel* tempModel = new Snowblind::CModel();
	tempModel->SetEffect(anEffect);
	//tempModel->myEffect = anEffect;
	if (someModelData->myData)
	{
		FillData(someModelData->myData, tempModel, anEffect);
		tempModel->myOrientation = someModelData->myOrientation;

	}
	for (int i = 0; i < someModelData->myChilds.Size(); ++i)
	{
		auto currentChild = someModelData->myChilds[i];
		tempModel->AddChild(CreateModel(currentChild, anEffect));
	}
	return tempModel;
}

Snowblind::CModel* FBXFactory::LoadModel(const char* aFilePath, Snowblind::CEffect* anEffect)
{
	myTimeManager->Update();
	myLoadTime = myTimeManager->GetTimer(myLoadTimer).GetTotalTime().GetMilliseconds();
	//CU::TimeManager::GetInstance()->Update();

	FBXData* found = 0;
	for (FBXData* data : myFBXData)
	{
		if (data->myPath.compare(aFilePath) == 0)
		{
			found = data;
			break;
		}
	}

	FbxModelData* modelData = nullptr;
	if (found)
	{
		modelData = found->myData;
	}
	else
	{
		FBXData *data = new FBXData();
		FbxModelData* fbxModelData = myLoader->loadModel(aFilePath);
		data->myData = fbxModelData;
		data->myPath = aFilePath;
		myFBXData.push_back(data);
		modelData = data->myData;
	}


	Snowblind::CModel* returnModel = CreateModel(modelData, anEffect);
	myTimeManager->Update();
	myLoadTime  = myTimeManager->GetTimer(myLoadTimer).GetTotalTime().GetMilliseconds() - myLoadTime;

	if (myLoadTime > 7000.f)
	{
		MODEL_LOG("[%s] took %f ms to load. Check if it's saved as binary. Check amount of triangles!", aFilePath, myLoadTime);
	}
	else
		MODEL_LOG("[%s] took %f ms to load", aFilePath, myLoadTime);

	return returnModel;
}

Snowblind::CModel* FBXFactory::LoadModel(const char* aFilePath, const char* anEffectPath)
{
	return LoadModel(aFilePath, Snowblind::CAssetsContainer::GetInstance()->GetEffect(anEffectPath));
}


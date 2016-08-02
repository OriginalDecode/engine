#include "stdafx.h"
#include "ModelImporter.h"
#include <DL_Debug.h>
#include "Model.h"

#include "VertexWrapper.h"
#include "IndexWrapper.h"
#include "Surface.h"
#include "Model.h"

#define TRIANGLE_VERTEX_COUNT 3
#define VERTEX_STRIDE 4
#define NORMAL_STRIDE 4
#define BINORMAL_STRIDE 4
#define TANGENT_STRIDE 4
#define SKINWEIGHT_STRIDE 4
#define BONEID_STRIDE 4
#define UV_STRIDE 2


CModelImporter::CModelImporter()
	: myEngine(Snowblind::CEngine::GetInstance())
	, myTimeManager(new CommonUtilities::TimeManager())
{
	myTimeManager->CreateTimer();
}


CModelImporter::~CModelImporter()
{
	SAFE_DELETE(myTimeManager);
}

Snowblind::CModel* CModelImporter::CreateModel(FBXModelData* someData, Snowblind::CEffect* anEffect)
{
	Snowblind::CModel* newModel = new Snowblind::CModel();
	newModel->SetEffect(anEffect);

	if (someData->myData)
	{
		FillData(someData->myData, newModel, anEffect);
		newModel->myOrientation = someData->myOrientation;
	}
	for (FBXModelData* child : someData->myChildren)
	{
		newModel->AddChild(CreateModel(child, anEffect));
	}

	return newModel;
}

Snowblind::CModel* CModelImporter::LoadModel(const std::string& aFilePath, const std::string& aEffectPath)
{
	return LoadModel(aFilePath, myEngine->GetEffect(aEffectPath))->CreateModel();
}

Snowblind::CModel* CModelImporter::LoadModel(const std::string& aFilePath, Snowblind::CEffect* anEffect)
{
	myTimeManager->Update();
	float loadTime = myTimeManager->GetTimer(0).GetTotalTime().GetMilliseconds();
	const aiScene* scene = importer.ReadFile(aFilePath, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_MakeLeftHanded); //MaxQuality, Quality, Fast
	DL_MESSAGE("%s", scene ? aFilePath : importer.GetErrorString());
	DL_ASSERT_EXP(scene, "ImportModel Failed. Could not read the requested file.");

	aiNode* rootNode = scene->mRootNode;
	FBXModelData* data = new FBXModelData();
	ProcessNode(rootNode, scene, data);
	Snowblind::CModel* toReturn = CreateModel(data, anEffect);

	loadTime = myTimeManager->GetTimer(0).GetTotalTime().GetMilliseconds() - loadTime;
	MODEL_LOG("%s took %fms to load. %s", aFilePath, loadTime, (loadTime < 7000.f) ? "" : "Check if it's saved as binary.");

	return toReturn;
}

void CModelImporter::FillData(ModelData* someData, Snowblind::CModel* out, Snowblind::CEffect* anEffect)
{
	Snowblind::SVertexIndexWrapper* indexWrapper = new Snowblind::SVertexIndexWrapper();
	indexWrapper->myFormat = DXGI_FORMAT_R32_UINT;
	u32* indexData = new u32[someData->myIndexCount];
	memcpy(indexData, someData->myIndicies, someData->myIndexCount * sizeof(u32));
	indexWrapper->myIndexData = (s8*)indexData;
	indexWrapper->mySize = someData->myIndexCount * sizeof(u32);
	out->myIndexData = indexWrapper;

	Snowblind::SVertexDataWrapper* vertexData = new Snowblind::SVertexDataWrapper();
	s32 sizeOfBuffer = someData->myVertexCount * someData->myVertexStride * sizeof(float);
	s8* vertexRawData = new s8[sizeOfBuffer];
	memcpy(vertexRawData, someData->myVertexBuffer, sizeOfBuffer);
	vertexData->myVertexData = vertexRawData;
	vertexData->myNrOfVertexes = someData->myVertexCount;
	vertexData->mySize = sizeOfBuffer;
	vertexData->myStride = someData->myVertexStride * sizeof(float);
	out->myVertexData = vertexData;

	Snowblind::CSurface* newSurface = new Snowblind::CSurface(0, someData->myVertexCount, 0
		, someData->myIndexCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	out->myModelStates[NULL_OBJECT] = FALSE;
	for (int i = 0; i < someData->myLayout.Size(); ++i)
	{
		auto currentLayout = someData->myLayout[i];
		D3D11_INPUT_ELEMENT_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_INPUT_ELEMENT_DESC));
		desc.SemanticIndex = 0;
		desc.AlignedByteOffset = currentLayout.myOffset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InputSlot = 0;
		desc.InstanceDataStepRate = 0;

		if (currentLayout.myType == ModelData::VERTEX_POS)
		{
			desc.SemanticName = "POSITION";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_NORMAL)
		{
			desc.SemanticName = "NORMAL";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_UV)
		{
			desc.SemanticName = "TEXCOORD";
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BINORMAL)
		{
			desc.SemanticName = "BINORMAL";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_TANGENT)
		{
			desc.SemanticName = "TANGENT";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_SKINWEIGHTS)
		{
			break;
			desc.SemanticName = "WEIGHTS";
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BONEID)
		{
			break;
			desc.SemanticName = "BONES";
			desc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
		}
		out->myVertexFormat.Add(desc);
	}


	//newSurface->AddTexture()
	/*
		for each texture in modeldata
		add new texture to surface
	*/

	out->mySurfaces.Add(newSurface);
}

//FBXModelData* CModelImporter::ProcessNode(aiNode* aNode, const aiScene* aScene)
//{
//	FBXModelData* data = new FBXModelData();
//	for (u32 i = 0; i < aNode->mNumMeshes; i++)
//	{
//		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
//		data->myChildren.Add(ProcessMesh(mesh, aScene));
//	}
//	for (u32 i = 0; i < aNode->mNumChildren; i++)
//	{
//		ProcessNode(aNode->mChildren[i], aScene, data->myChildren.GetLast());
//	}
//
//	return data;
//}

void CModelImporter::ProcessNode(aiNode* aNode, const aiScene* aScene, FBXModelData* someData)
{
	DL_ASSERT_EXP(someData, "Failed to process node. FBXModelData someData was null");
	for (u32 i = 0; i < aNode->mNumMeshes; i++)
	{
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		ProcessMesh(mesh, aScene, someData);
	}

	for (u32 i = 0; i < aNode->mNumChildren; i++)
	{
		someData->myChildren.Add(new FBXModelData());
		ProcessNode(aNode->mChildren[i], aScene, someData->myChildren.GetLast());
	}
}

void CModelImporter::ProcessMesh(aiMesh* aMesh, const aiScene* aScene, FBXModelData* fbx)
{
	FBXModelData* data = fbx;
	data->myData = new ModelData();

	data->myData->myVertexCount = aMesh->mNumVertices;
	u32 polygonCount = aMesh->mNumFaces;
	u32 size = polygonCount * 3;
	if (aMesh->HasPositions())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_POS;
		newLayout.mySize = VERTEX_STRIDE;
		newLayout.myOffset = 0;
		data->myData->myLayout.Add(newLayout);
	}
	u32 stride = VERTEX_STRIDE;


	if (aMesh->HasNormals())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_NORMAL;
		newLayout.mySize = NORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += NORMAL_STRIDE;
		size += polygonCount * NORMAL_STRIDE;
	}

	if (aMesh->HasTextureCoords(0))
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_UV;
		newLayout.mySize = UV_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += UV_STRIDE;
		size += polygonCount * UV_STRIDE;
	}

	if (aMesh->HasTangentsAndBitangents())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_BINORMAL;
		newLayout.mySize = BINORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += BINORMAL_STRIDE;
		size += polygonCount * BINORMAL_STRIDE;

		newLayout.myType = ModelData::VERTEX_TANGENT;
		newLayout.mySize = TANGENT_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += TANGENT_STRIDE;
		size += polygonCount * TANGENT_STRIDE;
	}

	if (aMesh->HasBones())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_SKINWEIGHTS;
		newLayout.mySize = SKINWEIGHT_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += SKINWEIGHT_STRIDE;
		size += polygonCount * SKINWEIGHT_STRIDE;

		newLayout.myType = ModelData::VERTEX_BONEID;
		newLayout.mySize = BONEID_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += BONEID_STRIDE;
		size += polygonCount * BONEID_STRIDE;
	}

	//data.myData.myVertexBuffer = new float[size];
	data->myData->myVertexBuffer = new float[size];
	for (u32 i = 0; i < aMesh->mNumVertices;)
	{
		data->myData->myVertexBuffer[i] = aMesh->mVertices[i].x;
		data->myData->myVertexBuffer[i + 1] = aMesh->mVertices[i].y;
		data->myData->myVertexBuffer[i + 2] = aMesh->mVertices[i].z;
		data->myData->myVertexBuffer[i + 3] = 0;
		u32 addedSize = VERTEX_STRIDE;

		data->myData->myVertexBuffer[i + addedSize] = aMesh->mNormals[i].x;
		data->myData->myVertexBuffer[i + addedSize + 1] = aMesh->mNormals[i].y;
		data->myData->myVertexBuffer[i + addedSize + 2] = aMesh->mNormals[i].z;
		data->myData->myVertexBuffer[i + addedSize + 3] = 0;
		addedSize += NORMAL_STRIDE;

		data->myData->myVertexBuffer[i + addedSize] = aMesh->mTextureCoords[0]->x;
		data->myData->myVertexBuffer[i + addedSize + 1] = aMesh->mTextureCoords[0]->y * -1.f;
		addedSize += UV_STRIDE;

		//vertexData[i + addedSize] = aMesh->



		//aMesh->mVertices[i]
		/*
			Create and Add a vertice to the vertice list, does this really do that tho?
		*/
		i += addedSize;
	}
	

	data->myData->myVertexStride = stride;

	CU::GrowingArray<u32> indexes;
	for (u32 i = 0; i < aMesh->mNumFaces; i++)
	{
		aiFace face = aMesh->mFaces[i];

		for (u32 j = 0; j < face.mNumIndices; j++)
		{
			indexes.Add(face.mIndices[j]);
		}
	}


	//data->myData->myIndicies = new u32[indexes.Size()];
	data->myData->myIndicies = new u32[indexes.Size()];
	ZeroMemory(data->myData->myIndicies, sizeof(u32) * indexes.Size());
	for (u32 i = 0; i < indexes.Size(); i++)
	{
		data->myData->myIndicies[i] = indexes[i];
	}

	data->myData->myIndexCount = indexes.Size();
}

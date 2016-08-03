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
	const aiScene* scene = importer.ReadFile(aFilePath, aiProcess_SplitLargeMeshes | aiProcess_MakeLeftHanded | aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_Triangulate); //MaxQuality, Quality, Fast 
	DL_MESSAGE("%s", !scene ? aFilePath.c_str() : importer.GetErrorString());
	DL_ASSERT_EXP(scene, "ImportModel Failed. Could not read the requested file.");

	aiNode* rootNode = scene->mRootNode;
	FBXModelData* data = new FBXModelData();
	ProcessNode(rootNode, scene, data);
	Snowblind::CModel* toReturn = CreateModel(data, anEffect);

	delete data->myData;
	delete data;

	loadTime = myTimeManager->GetTimer(0).GetTotalTime().GetMilliseconds() - loadTime;
	MODEL_LOG("%s took %fms to load. %s", aFilePath.c_str(), loadTime, (loadTime < 7000.f) ? "" : "Check if it's saved as binary.");

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

	/* BUG HERE. CRASH. */
	Snowblind::SVertexDataWrapper* vertexData = new Snowblind::SVertexDataWrapper();
	s32 sizeOfBuffer = someData->myVertexCount * someData->myVertexStride * sizeof(float); //is this wrong?
	u32* vertexRawData = new u32[sizeOfBuffer];
	memcpy(vertexRawData, someData->myVertexBuffer, sizeOfBuffer); // This crashes?
	vertexData->myVertexData = (s8*)vertexRawData;
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
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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


	u32 polygonCount = aMesh->mNumFaces;
	u32 size = polygonCount * VERTEX_STRIDE;
	u32 polygonVertexCount = polygonCount * 4;

	if (aMesh->HasPositions())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_POS;
		newLayout.mySize = VERTEX_STRIDE;
		newLayout.myOffset = 0;
		data->myData->myLayout.Add(newLayout);
		size += polygonVertexCount * VERTEX_STRIDE;
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
		size += polygonVertexCount * NORMAL_STRIDE;
	}

	if (aMesh->HasTextureCoords(0))
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_UV;
		newLayout.mySize = UV_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += UV_STRIDE;
		size += polygonVertexCount * UV_STRIDE;
	}

	if (aMesh->HasTangentsAndBitangents())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_BINORMAL;
		newLayout.mySize = BINORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += BINORMAL_STRIDE;
		size += polygonVertexCount * BINORMAL_STRIDE;

		newLayout.myType = ModelData::VERTEX_TANGENT;
		newLayout.mySize = TANGENT_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += TANGENT_STRIDE;
		size += polygonVertexCount * TANGENT_STRIDE;
	}

	if (aMesh->HasBones())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_SKINWEIGHTS;
		newLayout.mySize = SKINWEIGHT_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += SKINWEIGHT_STRIDE;
		size += polygonVertexCount * SKINWEIGHT_STRIDE;

		newLayout.myType = ModelData::VERTEX_BONEID;
		newLayout.mySize = BONEID_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += BONEID_STRIDE;
		size += polygonVertexCount * BONEID_STRIDE;
	}

	DL_MESSAGE("Vertex Buffer Array Size : %d", size);
	data->myData->myVertexBuffer = new float[size];
	ZeroMemory(data->myData->myVertexBuffer, sizeof(float)*size);

	DL_ASSERT_EXP(aMesh->mNumVertices < size, "the amount of vertices was MORE!? than size");
	u32 vertexCount;
	for (u32 i = 0; i < aMesh->mNumVertices; i++)
	{
		u32 addedSize = VERTEX_STRIDE;
		u32 currIndex = i * stride;
		u32 rest = addedSize % 18;
		DL_MESSAGE("\nAddedSize : %d\nrest : %d\nIndex : %d", addedSize, rest, i);

		DL_ASSERT_EXP(addedSize <= size, "addedSize was larger than the size of the array.");

		if (aMesh->HasPositions())
		{
			data->myData->myVertexBuffer[currIndex] = aMesh->mVertices[i].x;
			data->myData->myVertexBuffer[currIndex + 1] = aMesh->mVertices[i].y;
			data->myData->myVertexBuffer[currIndex + 2] = aMesh->mVertices[i].z;
			data->myData->myVertexBuffer[currIndex + 3] = 0;
		}

		if (aMesh->HasNormals())
		{
			data->myData->myVertexBuffer[currIndex + addedSize] = aMesh->mNormals[i].x;
			data->myData->myVertexBuffer[currIndex + addedSize + 1] = aMesh->mNormals[i].y;
			data->myData->myVertexBuffer[currIndex + addedSize + 2] = aMesh->mNormals[i].z;
			data->myData->myVertexBuffer[currIndex + addedSize + 3] = 0;
			addedSize += NORMAL_STRIDE;
		}

		if (aMesh->HasTextureCoords(0))
		{
			data->myData->myVertexBuffer[currIndex + addedSize] = aMesh->mTextureCoords[0][i].x;
			data->myData->myVertexBuffer[currIndex + addedSize + 1] = aMesh->mTextureCoords[0][i].y * -1.f;
			addedSize += UV_STRIDE;
		}

		if (aMesh->HasTangentsAndBitangents())
		{
			data->myData->myVertexBuffer[currIndex + addedSize] = aMesh->mBitangents[i].x;
			data->myData->myVertexBuffer[currIndex + addedSize + 1] = aMesh->mBitangents[i].y;
			data->myData->myVertexBuffer[currIndex + addedSize + 2] = aMesh->mBitangents[i].z;
			data->myData->myVertexBuffer[currIndex + addedSize + 3] = 0;
			addedSize += BINORMAL_STRIDE;

			data->myData->myVertexBuffer[currIndex + addedSize] = aMesh->mTangents[i].x;
			data->myData->myVertexBuffer[currIndex + addedSize + 1] = aMesh->mTangents[i].y;
			data->myData->myVertexBuffer[currIndex + addedSize + 2] = aMesh->mTangents[i].z;
			data->myData->myVertexBuffer[currIndex + addedSize + 3] = 0;
			addedSize += TANGENT_STRIDE;
		}
	}

	data->myData->myVertexCount = aMesh->mNumVertices;
	data->myData->myVertexStride = stride;
	data->myData->myIndicies = new u32[polygonCount * 3];
	ZeroMemory(data->myData->myIndicies, sizeof(u32) * polygonCount * 3);

	for (u32 i = 0; i < aMesh->mNumFaces; i++)
	{
		aiFace face = aMesh->mFaces[i];
		for (u32 j = 0; j < face.mNumIndices; j++)
		{
			data->myData->myIndicies[i + j] = face.mIndices[j];
		}
	}
	data->myData->myIndexCount = polygonCount * 3;
}

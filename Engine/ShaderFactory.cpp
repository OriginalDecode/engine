#include "stdafx.h"
#include "ShaderFactory.h"
#include <d3dcompiler.h>
#include <Utilities.h>
#include <JSON/JSONReader.h>
#include "ShaderWarningHandler.h"
#define ITTERATE(shadermap) auto it = shadermap.begin(); it != shadermap.end(); it++

#define VERTEX 0
#define PIXEL 1
#define GEOMETRY 2
#define HULL 3
#define DOMAINS 4
#define COMPUTE 5

namespace Snowblind
{
	CShaderFactory::CShaderFactory()
		: myFileWatchers(6)
	{
#if defined (_DEBUG)
		for (int i = 0; i < 6; i++)
		{
			FileWatcher* watcher = new FileWatcher();
			myFileWatchers.Add(watcher);
		}
		myShaderWarningHandler = new ShaderWarningHandler();
#endif
	}

	CShaderFactory::~CShaderFactory()
	{
		myFileWatchers.DeleteAll();
		SAFE_DELETE(myShaderWarningHandler);
		for (ITTERATE(myVertexShaders))
		{
			SAFE_DELETE(it->second);
		}
		for (ITTERATE(myPixelShaders))
		{
			SAFE_DELETE(it->second);
		}
		for (ITTERATE(myGeometryShaders))
		{
			SAFE_DELETE(it->second);
		}
		for (ITTERATE(myHullShaders))
		{
			SAFE_DELETE(it->second);
		}
		for (ITTERATE(myDomainShaders))
		{
			SAFE_DELETE(it->second);
		}
		for (ITTERATE(myComputeShaders))
		{
			SAFE_DELETE(it->second);
		}
	}

	void CShaderFactory::LoadShader(CEffect* anEffect)
	{
		std::string path = anEffect->myFileName;
		std::string sub = CL::substr(path, "/", true, 0);


		std::string vertexShader;
		std::string pixelShader;
		std::string geometryShader;
		std::string hullShader;
		std::string domainShader;
		std::string computeShader;

		JSONReader reader(path);
		reader.ReadElement("VertexShader", vertexShader);
		reader.ReadElement("PixelShader", pixelShader);
		reader.ReadElement("GeometryShader", geometryShader);
		reader.ReadElement("HullShader", hullShader);
		reader.ReadElement("DomainShader", domainShader);
		reader.ReadElement("ComputeShader", computeShader);

		std::string input(sub + "/" + vertexShader);
		if (vertexShader != "")
		{
			LoadShader(input, anEffect->myVertexShader);
			myVertexShaders[input]->effectPointers.Add(anEffect);
		}

		input.clear();
		input = sub + "/" + pixelShader;
		if (pixelShader != "")
		{
			LoadShader(input, anEffect->myPixelShader);
			if (myPixelShaders[input] != nullptr)
			{
				myPixelShaders[input]->effectPointers.Add(anEffect);
			}
		}

		input.clear();
		input = sub + "/" + geometryShader;
		if (geometryShader != "")
		{
			LoadShader(input, anEffect->myGeometryShader);
			myGeometryShaders[input]->effectPointers.Add(anEffect);
		}

		input.clear();
		input = sub + "/" + hullShader;
		if (hullShader != "")
		{
			LoadShader(input, anEffect->myHullShader);
			myHullShaders[input]->effectPointers.Add(anEffect);
		}

		input.clear();
		input = sub + "/" + domainShader;
		if (domainShader != "")
		{
			LoadShader(input, anEffect->myDomainShader);
			myDomainShaders[input]->effectPointers.Add(anEffect);
		}

		input.clear();
		input = sub + "/" + computeShader;
		if (computeShader != "")
		{
			LoadShader(input, anEffect->myComputeShader);
			myComputeShaders[input]->effectPointers.Add(anEffect);
		}

	}

	//----------------------------------------
	// Vertex Shader
	//----------------------------------------
	void CShaderFactory::LoadShader(const std::string& aShader, SVertexShader*& aVertexShader)
	{
		if (myVertexShaders.find(aShader) == myVertexShaders.end())
		{
			CreateVertexShader(aShader);
#if defined (_DEBUG)
			myFileWatchers[VERTEX]->WatchFileChangeWithDependencies(aShader, std::bind(&CShaderFactory::ReloadVertex, this, std::placeholders::_1));
#endif
		}
		aVertexShader = myVertexShaders[aShader];
	}

	void CShaderFactory::CreateVertexShader(const std::string& aShader)
	{
		SVertexShader* newShader = new SVertexShader();
		ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

		ENGINE_LOG("Creating vertexshader %s", aShader.c_str());
		HRESULT hr;

		unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
		shaderFlag |= D3D10_SHADER_DEBUG;
		shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
		ID3DBlob* compiledShader = 0;
		ID3DBlob* compilationMessage = 0;

		std::wstring fileName(aShader.begin(), aShader.end());
		hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "VS", "vs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);

		if (compilationMessage != nullptr)
		{
			std::string msg = myShaderWarningHandler->CheckWarning((char*)compilationMessage->GetBufferPointer(), aShader);
			DL_WARNING("%s", msg.c_str());
		}
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &newShader->vertexShader);

		newShader->blob = compiledShader;
		newShader->compiledShader = compiledShader->GetBufferPointer();
		newShader->shaderSize = compiledShader->GetBufferSize();

		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(newShader->vertexShader, "VertexShader");

		myVertexShaders[aShader] = newShader;
	}

	void CShaderFactory::ReloadVertex(const std::string& aFilePath)
	{
		Sleep(10);
		CU::GrowingArray<CEffect*> effectPointers = GetEffectArray(aFilePath);
		CreateVertexShader(aFilePath);
		for each (CEffect* effect in effectPointers)
		{
			effect->myVertexShader = myVertexShaders[aFilePath];
			DL_ASSERT_EXP(effect->myVertexShader != nullptr, "Vertex Shader pointer was null. Something failed on Shader Reload.");
			myVertexShaders[aFilePath]->effectPointers.Add(effect);
		}
	}

	//----------------------------------------
	// Pixel Shader
	//----------------------------------------
	void CShaderFactory::LoadShader(const std::string& aShader, SPixelShader*& aPixelShader)
	{
		if (myPixelShaders.find(aShader) == myPixelShaders.end())
		{
			CreatePixelShader(aShader);
#if defined (_DEBUG)
			myFileWatchers[PIXEL]->WatchFileChangeWithDependencies(aShader, std::bind(&CShaderFactory::ReloadPixel, this,std::placeholders::_1));
#endif
		}
		aPixelShader = myPixelShaders[aShader];
	}

	void CShaderFactory::CreatePixelShader(const std::string& aShader)
	{
		SPixelShader* newShader = new SPixelShader();
		myPixelShaders[aShader] = newShader;
		ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

		ENGINE_LOG("Creating pixelshader : %s", aShader.c_str());
		HRESULT hr;
		unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
		shaderFlag |= D3D10_SHADER_DEBUG;
		shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = 0;
		ID3D10Blob* compilationMessage = 0;

		std::wstring fileName(aShader.begin(), aShader.end());

		hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "PS", "ps_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);
		if (compilationMessage != nullptr)
		{
			std::string msg = myShaderWarningHandler->CheckWarning((char*)compilationMessage->GetBufferPointer(), aShader);
			DL_WARNING("%s", msg.c_str());
		}

		if (FAILED(hr))
		{
			DL_WARNINGBOX(myShaderWarningHandler->CheckWarning((char*)compilationMessage->GetBufferPointer(), aShader).c_str());
			return;
		}
		//CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &newShader->pixelShader);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Pixel Shader.");

		newShader->blob = compiledShader;
		newShader->compiledShader = compiledShader->GetBufferPointer();
		newShader->shaderSize = compiledShader->GetBufferSize();

		CEngine::GetDirectX()->SetDebugName(newShader->pixelShader, "PixelShader");

	}

	void CShaderFactory::ReloadPixel(const std::string& aFilePath)
	{
		Sleep(10);
		CU::GrowingArray<CEffect*> effectPointers = GetEffectArray(aFilePath);
		CreatePixelShader(aFilePath);
		for each (CEffect* effect in effectPointers)
		{
			effect->myPixelShader = myPixelShaders[aFilePath];
			DL_ASSERT_EXP(effect->myPixelShader != nullptr, "Vertex Shader pointer was null. Something failed on Shader Reload.");
			myPixelShaders[aFilePath]->effectPointers.Add(effect);
		}
	}

	//----------------------------------------
	// Geometry Shader
	//----------------------------------------
	void CShaderFactory::LoadShader(const std::string& aShader, SGeometryShader*& aGeometryShader)
	{
		if (myGeometryShaders.find(aShader) == myGeometryShaders.end())
		{
			SGeometryShader* newShader = new SGeometryShader();
			ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

			ENGINE_LOG("Creating geometryshader : %s", aShader.c_str());
			HRESULT hr;
			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
			shaderFlag |= D3D10_SHADER_DEBUG;
			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

			ID3D10Blob* compiledShader = 0;
			ID3D10Blob* compilationMessage = 0;
			std::wstring fileName(aShader.begin(), aShader.end());
			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "GS", "gs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);

			if (compilationMessage != nullptr)
			{
				std::string msg = myShaderWarningHandler->CheckWarning((char*)compilationMessage->GetBufferPointer(), aShader);
				DL_WARNING("%s", msg.c_str());
			}

			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

			hr = device->CreateGeometryShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &newShader->geometryShader);
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Geometry Shader.");
			newShader->blob = compiledShader;
			newShader->compiledShader = compiledShader->GetBufferPointer();
			newShader->shaderSize = compiledShader->GetBufferSize();
			CEngine::GetDirectX()->SetDebugName(newShader->geometryShader, "GeometryShader");
			myGeometryShaders[aShader] = newShader;
		}
		aGeometryShader = myGeometryShaders[aShader];
	}

	//----------------------------------------
	// Hull Shader
	//----------------------------------------
	void CShaderFactory::LoadShader(const std::string& aShader, SHullShader*& aHullShader)
	{
		if (myHullShaders.find(aShader) == myHullShaders.end())
		{
			SHullShader* newShader = new SHullShader();
			ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

			ENGINE_LOG("Creating hullshader : %s", aShader.c_str());
			HRESULT hr;
			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
			shaderFlag |= D3D10_SHADER_DEBUG;
			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

			ID3D10Blob* compiledShader = 0;
			ID3D10Blob* compilationMessage = 0;

			std::wstring fileName(aShader.begin(), aShader.end());
			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "HS", "hs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);
			if (compilationMessage != nullptr)
			{
				std::string msg = myShaderWarningHandler->CheckWarning((char*)compilationMessage->GetBufferPointer(), aShader);
				DL_WARNING("%s", msg.c_str());
			}

			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

			hr = device->CreateHullShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &newShader->hullShader);
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Hull Shader.");
			newShader->blob = compiledShader;
			newShader->compiledShader = compiledShader->GetBufferPointer();
			newShader->shaderSize = compiledShader->GetBufferSize();
			CEngine::GetDirectX()->SetDebugName(newShader->hullShader, "HullShader");
			myHullShaders[aShader] = newShader;
		}
		aHullShader = myHullShaders[aShader];
	}

	//----------------------------------------
	// Domain Shader
	//----------------------------------------
	void CShaderFactory::LoadShader(const std::string& aShader, SDomainShader*& aDomainShader)
	{

		if (myDomainShaders.find(aShader) == myDomainShaders.end())
		{
			SDomainShader* newShader = new SDomainShader();
			ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

			ENGINE_LOG("Creating domainshader : %s", aShader.c_str());
			HRESULT hr;
			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
			shaderFlag |= D3D10_SHADER_DEBUG;
			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

			ID3D10Blob* compiledShader = 0;
			ID3D10Blob* compilationMessage = 0;

			std::wstring fileName(aShader.begin(), aShader.end());
			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "DS", "ds_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);

			if (compilationMessage != nullptr)
			{
				std::string msg = myShaderWarningHandler->CheckWarning((char*)compilationMessage->GetBufferPointer(), aShader);
				DL_WARNING("%s", msg.c_str());
			}

			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

			hr = device->CreateDomainShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &newShader->domainShader);
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Domain Shader.");
			newShader->blob = compiledShader;
			newShader->compiledShader = compiledShader->GetBufferPointer();
			newShader->shaderSize = compiledShader->GetBufferSize();
			CEngine::GetDirectX()->SetDebugName(newShader->domainShader, "DomainShader");
			myDomainShaders[aShader] = newShader;
		}
		aDomainShader = myDomainShaders[aShader];
	}

	//----------------------------------------
	// Compute Shader
	//----------------------------------------
	void CShaderFactory::LoadShader(const std::string& aShader, SComputeShader*& aComputeShader)
	{
		if (myComputeShaders.find(aShader) == myComputeShaders.end())
		{
			SComputeShader* newShader = new SComputeShader();
			ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

			ENGINE_LOG("Creating computeshader : %s", aShader.c_str());
			HRESULT hr;
			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
			shaderFlag |= D3D10_SHADER_DEBUG;
			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

			ID3D10Blob* compiledShader = 0;
			ID3D10Blob* compilationMessage = 0;

			std::wstring fileName(aShader.begin(), aShader.end());
			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "CS", "cs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);

			if (compilationMessage != nullptr)
			{
				std::string msg = myShaderWarningHandler->CheckWarning((char*)compilationMessage->GetBufferPointer(), aShader);
				DL_WARNING("%s", msg.c_str());
			}

			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

			hr = device->CreateComputeShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &newShader->computeShader);
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Compute Shader.");
			newShader->blob = compiledShader;
			newShader->compiledShader = compiledShader->GetBufferPointer();
			newShader->shaderSize = compiledShader->GetBufferSize();
			CEngine::GetDirectX()->SetDebugName(newShader->computeShader, "ComputeShader");

			myComputeShaders[aShader] = newShader;
		}
		aComputeShader = myComputeShaders[aShader];
	}
	

	CU::GrowingArray<CEffect*> CShaderFactory::GetEffectArray(const std::string& aFilePath)
	{
		CU::GrowingArray<CEffect*> effectPointers;

		//----------------------------------------
		// Vertex Shader
		//----------------------------------------
		if (myVertexShaders.find(aFilePath) != myVertexShaders.end())
		{
			for each(CEffect* effect in myVertexShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myVertexShaders[aFilePath];
			myVertexShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		//----------------------------------------
		// Pixel Shader
		//----------------------------------------
		if (myPixelShaders.find(aFilePath) != myPixelShaders.end())
		{
			for each(CEffect* effect in myPixelShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myPixelShaders[aFilePath];
			myPixelShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		//----------------------------------------
		// Geometry Shader
		//----------------------------------------
		if (myGeometryShaders.find(aFilePath) != myGeometryShaders.end())
		{
			for each(CEffect* effect in myGeometryShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myGeometryShaders[aFilePath];
			myGeometryShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		//----------------------------------------
		// Hull Shader
		//----------------------------------------
		if (myHullShaders.find(aFilePath) != myHullShaders.end())
		{
			for each(CEffect* effect in myHullShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myHullShaders[aFilePath];
			myHullShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		//----------------------------------------
		// Domain Shader
		//----------------------------------------
		if (myDomainShaders.find(aFilePath) != myDomainShaders.end())
		{
			for each(CEffect* effect in myDomainShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myDomainShaders[aFilePath];
			myDomainShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		//----------------------------------------
		// Compute Shader
		//----------------------------------------
		if (myComputeShaders.find(aFilePath) != myComputeShaders.end())
		{
			for each(CEffect* effect in myComputeShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myComputeShaders[aFilePath];
			myComputeShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		DL_ASSERT("Failed to find effect. Reloading something that doesn't exist?");
		return 0;
	}

	void CShaderFactory::Update()
	{
		for each(FileWatcher* watcher in myFileWatchers)
		{
			watcher->FlushChanges();
		}
	}

	//----------------------------------------
	// Shader Structs
	//----------------------------------------

	SCompiledShader::~SCompiledShader()
	{
		SAFE_RELEASE(blob);
		compiledShader = nullptr;
	}

	SVertexShader::~SVertexShader()
	{
		SAFE_RELEASE(vertexShader);
	}

	SPixelShader::~SPixelShader()
	{
		SAFE_RELEASE(pixelShader);
	}

	SGeometryShader::~SGeometryShader()
	{
		SAFE_RELEASE(geometryShader);
	}

	SHullShader::~SHullShader()
	{
		SAFE_RELEASE(hullShader);
	}

	SDomainShader::~SDomainShader()
	{
		SAFE_RELEASE(domainShader);
	}

	SComputeShader::~SComputeShader()
	{
		SAFE_RELEASE(computeShader);
	}

};
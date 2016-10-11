#include "stdafx.h"
#include "ShaderFactory.h"
#include <d3dcompiler.h>
#include <Utilities.h>
#include <JSON/JSONReader.h>


namespace Snowblind
{
	CShaderFactory::CShaderFactory()
	{
		for (int i = 0; i < 6; i++)
		{
			FileWatcher* watcher = new FileWatcher();
			myFileWatchers.Insert(i, watcher);
		}
	}

	CShaderFactory::~CShaderFactory()
	{
		myFileWatchers.DeleteAll();
		DELETE_MAP(myVertexShaders);
		DELETE_MAP(myPixelShaders);
		DELETE_MAP(myGeometryShaders);
		DELETE_MAP(myHullShaders);
		DELETE_MAP(myDomainShaders);
		DELETE_MAP(myComputeShaders);
	}

	void CShaderFactory::LoadShader(CEffect* anEffect)
	{
		std::string path = anEffect->myFileName;
		std::string sub = CL::substr(path, "/", true, 0) + "/";

		JSONReader reader(path);
		/*	std::string geometry_shader = reader.ReadElement("GeometryShader");
			std::string hull_shader = reader.ReadElement("HullShader");
			std::string domain_shader = reader.ReadElement("DomainShader");
			std::string compute_shader = reader.ReadElement("ComputeShader");*/

		LoadVertexShader(sub + reader.ReadElement("VertexShader"), anEffect);
		LoadPixelShader(sub + reader.ReadElement("PixelShader"), anEffect);
	}

	//----------------------------------------
	// Vertex Shader
	//----------------------------------------
	bool CShaderFactory::LoadVertexShader(const std::string& file_path, CEffect* effect)
	{
		if (myVertexShaders.find(file_path) == myVertexShaders.end())
		{
			myVertexShaders[file_path] = CreateVertexShader(file_path);
			if (!myVertexShaders[file_path])
				return false;

			myFileWatchers[u32(eShaderType::VERTEX)]->WatchFileChangeWithDependencies(file_path, std::bind(&CShaderFactory::ReloadVertex, this, std::placeholders::_1));
		}
		effect->myVertexShader = myVertexShaders[file_path];
		myVertexShaders[file_path]->effectPointers.Add(effect);
		return true;
	}

	SVertexShader* CShaderFactory::CreateVertexShader(const std::string& file_path)
	{
		SVertexShader* newShader = new SVertexShader();
#ifndef SNOWBLIND_VULKAN
		ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

		ENGINE_LOG("Creating vertexshader %s", file_path.c_str());
		HRESULT hr;

		ID3D10Blob* compiled_shader = CompileShader(file_path, "VS", "vs_5_0");
		hr = device->CreateVertexShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &newShader->vertexShader);

		newShader->blob = compiled_shader;
		newShader->compiledShader = compiled_shader->GetBufferPointer();
		newShader->shaderSize = compiled_shader->GetBufferSize();

		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(newShader->vertexShader, "VertexShader");
#endif
		return newShader;
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
	void CShaderFactory::LoadPixelShader(const std::string& file_path, CEffect* effect)
	{
		if (myPixelShaders.find(file_path) == myPixelShaders.end())
		{
			myPixelShaders[file_path] = CreatePixelShader(file_path);
			myFileWatchers[u32(eShaderType::PIXEL)]->WatchFileChangeWithDependencies(file_path, std::bind(&CShaderFactory::ReloadPixel, this,std::placeholders::_1));
		}

		effect->myPixelShader = myPixelShaders[file_path];
		myPixelShaders[file_path]->effectPointers.Add(effect);
	}

	SPixelShader* CShaderFactory::CreatePixelShader(const std::string& file_path)
	{
		SPixelShader* newShader = new SPixelShader();
#ifndef SNOWBLIND_VULKAN
		myPixelShaders[file_path] = newShader;
		ID3D11Device* device = CEngine::GetDirectX()->GetDevice();
		ENGINE_LOG("Creating pixelshader : %s", file_path.c_str());
		
		HRESULT hr;
		ID3D10Blob* compiled_shader = CompileShader(file_path, "PS", "ps_5_0");

		hr = device->CreatePixelShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &newShader->pixelShader);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Pixel Shader.");

		newShader->blob = compiled_shader;
		newShader->compiledShader = compiled_shader->GetBufferPointer();
		newShader->shaderSize = compiled_shader->GetBufferSize();

		CEngine::GetDirectX()->SetDebugName(newShader->pixelShader, "PixelShader");
#endif
		return newShader;
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
	void CShaderFactory::LoadGeometryShader(const std::string& file_path, CEffect* effect)
	{
#ifndef SNOWBLIND_VULKAN
		if (myGeometryShaders.find(file_path) == myGeometryShaders.end())
		{
			SGeometryShader* newShader = new SGeometryShader();
			ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

			ENGINE_LOG("Creating geometryshader : %s", file_path.c_str());
			HRESULT hr;
			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
			shaderFlag |= D3D10_SHADER_DEBUG;
			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

			ID3D10Blob* compiledShader = 0;
			ID3D10Blob* compilationMessage = 0;
			std::wstring fileName(file_path.begin(), file_path.end());
			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "GS", "gs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);
			if (compilationMessage != nullptr)
			{
				std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
				DL_WARNING("%s", msg.c_str());
			}

			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

			hr = device->CreateGeometryShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &newShader->geometryShader);
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Geometry Shader.");
			newShader->blob = compiledShader;
			newShader->compiledShader = compiledShader->GetBufferPointer();
			newShader->shaderSize = compiledShader->GetBufferSize();
			CEngine::GetDirectX()->SetDebugName(newShader->geometryShader, "GeometryShader");
			myGeometryShaders[file_path] = newShader;
		}
		effect->myGeometryShader = myGeometryShaders[file_path];
#endif
	}

	//----------------------------------------
	// Hull Shader
	//----------------------------------------
	void CShaderFactory::LoadHullShader(const std::string& file_path, CEffect* effect)
	{
#ifndef SNOWBLIND_VULKAN
		if (myHullShaders.find(file_path) == myHullShaders.end())
		{
			SHullShader* newShader = new SHullShader();
			ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

			ENGINE_LOG("Creating hullshader : %s", file_path.c_str());
			HRESULT hr;
			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
			shaderFlag |= D3D10_SHADER_DEBUG;
			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

			ID3D10Blob* compiledShader = 0;
			ID3D10Blob* compilationMessage = 0;

			std::wstring fileName(file_path.begin(), file_path.end());
			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "HS", "hs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);
			if (compilationMessage != nullptr)
			{
				std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
				DL_WARNING("%s", msg.c_str());
			}

			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

			hr = device->CreateHullShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &newShader->hullShader);
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Hull Shader.");
			newShader->blob = compiledShader;
			newShader->compiledShader = compiledShader->GetBufferPointer();
			newShader->shaderSize = compiledShader->GetBufferSize();
			CEngine::GetDirectX()->SetDebugName(newShader->hullShader, "HullShader");
			myHullShaders[file_path] = newShader;
		}
		effect->myHullShader = myHullShaders[file_path];
#endif
	}

	//----------------------------------------
	// Domain Shader
	//----------------------------------------
	void CShaderFactory::LoadDomainShader(const std::string& file_path, CEffect* effect)
	{

#ifndef SNOWBLIND_VULKAN
		if (myDomainShaders.find(file_path) == myDomainShaders.end())
		{
			SDomainShader* newShader = new SDomainShader();
			ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

			ENGINE_LOG("Creating domainshader : %s", file_path.c_str());
			HRESULT hr;
			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
			shaderFlag |= D3D10_SHADER_DEBUG;
			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

			ID3D10Blob* compiledShader = 0;
			ID3D10Blob* compilationMessage = 0;

			std::wstring fileName(file_path.begin(), file_path.end());
			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "DS", "ds_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);

			if (compilationMessage != nullptr)
			{
				std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
				DL_WARNING("%s", msg.c_str());
			}

			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

			hr = device->CreateDomainShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &newShader->domainShader);
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Domain Shader.");
			newShader->blob = compiledShader;
			newShader->compiledShader = compiledShader->GetBufferPointer();
			newShader->shaderSize = compiledShader->GetBufferSize();
			CEngine::GetDirectX()->SetDebugName(newShader->domainShader, "DomainShader");
			myDomainShaders[file_path] = newShader;
		}
		effect->myDomainShader = myDomainShaders[file_path];
#endif
	}

	//----------------------------------------
	// Compute Shader
	//----------------------------------------
	void CShaderFactory::LoadComputeShader(const std::string& file_path, CEffect* effect)
	{
#ifndef SNOWBLIND_VULKAN
		if (myComputeShaders.find(file_path) == myComputeShaders.end())
		{
			SComputeShader* newShader = new SComputeShader();
			ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

			ENGINE_LOG("Creating computeshader : %s", file_path.c_str());
			HRESULT hr;
			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
			shaderFlag |= D3D10_SHADER_DEBUG;
			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

			ID3D10Blob* compiledShader = 0;
			ID3D10Blob* compilationMessage = 0;

			std::wstring fileName(file_path.begin(), file_path.end());
			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "CS", "cs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);

			if (compilationMessage != nullptr)
			{
				std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
				DL_WARNING("%s", msg.c_str());
			}

			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

			hr = device->CreateComputeShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &newShader->computeShader);
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Compute Shader.");
			newShader->blob = compiledShader;
			newShader->compiledShader = compiledShader->GetBufferPointer();
			newShader->shaderSize = compiledShader->GetBufferSize();
			CEngine::GetDirectX()->SetDebugName(newShader->computeShader, "ComputeShader");

			myComputeShaders[file_path] = newShader;
		}
		effect->myComputeShader = myComputeShaders[file_path];
#endif
	}
	

#ifndef SNOWBLIND_VULKAN
	ID3D10Blob* CShaderFactory::CompileShader(const std::string& file_path, const std::string& shader_type, const std::string& feature_level)
	{
		HRESULT hr;
		unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
		shaderFlag |= D3D10_SHADER_DEBUG;
		shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = 0;
		ID3D10Blob* compilationMessage = 0;
		std::wstring fileName(file_path.begin(), file_path.end());

		hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, shader_type.c_str(), feature_level.c_str(), shaderFlag, NULL, &compiledShader, &compilationMessage);
		if (compilationMessage != nullptr)
		{
			std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
			DL_WARNING("%s", msg.c_str());
		}
		if (FAILED(hr))
		{
			DL_WARNINGBOX(myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path).c_str());
		}

		return compiledShader;
	}
#endif

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
#ifndef SNOWBLIND_VULKAN
		SAFE_RELEASE(blob);
#endif
		compiledShader = nullptr;
	}

	SVertexShader::~SVertexShader()
	{
#ifndef SNOWBLIND_VULKAN
		SAFE_RELEASE(vertexShader);
#endif
	}

	SPixelShader::~SPixelShader()
	{
#ifndef SNOWBLIND_VULKAN
		SAFE_RELEASE(pixelShader);
#endif
	}

	SGeometryShader::~SGeometryShader()
	{
#ifndef SNOWBLIND_VULKAN
		SAFE_RELEASE(geometryShader);
#endif
	}

	SHullShader::~SHullShader()
	{
#ifndef SNOWBLIND_VULKAN
		SAFE_RELEASE(hullShader);
#endif
	}

	SDomainShader::~SDomainShader()
	{
#ifndef SNOWBLIND_VULKAN
		SAFE_RELEASE(domainShader);
#endif
	}

	SComputeShader::~SComputeShader()
	{
#ifndef SNOWBLIND_VULKAN
		SAFE_RELEASE(computeShader);
#endif
	}

};
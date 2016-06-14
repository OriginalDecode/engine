#include "stdafx.h"
#include "ShaderContainer.h"
#include <DataStructures/GrowingArray.h>
#include <d3dcompiler.h>
#include <vector>

#define ITTERATE(type) auto it = type.begin(); it != type.end(); it++

namespace Snowblind
{
	CShaderContainer* CShaderContainer::myInstance = nullptr;

	CShaderContainer::CShaderContainer()
	{
	}

	CShaderContainer::~CShaderContainer()
	{
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

	void CShaderContainer::LoadVertexShader(const std::string& aVertexShader)
	{
		SVertexShader* shader = new SVertexShader();
		CreateShader(aVertexShader, shader);
		myVertexShaders[aVertexShader] = shader;
	}

	void CShaderContainer::LoadPixelShader(const std::string& aPixelShader)
	{
		SPixelShader* shader = new SPixelShader();
		CreateShader(aPixelShader, shader);
		myPixelShaders[aPixelShader] = shader;
	}

	void CShaderContainer::LoadGeometryShader(const std::string& aGeometryShader)
	{
		SGeometryShader* shader = new SGeometryShader();
		CreateShader(aGeometryShader, shader);
		myGeometryShaders[aGeometryShader] = shader;
	}

	void CShaderContainer::LoadHullShader(const std::string& aHullShader)
	{
		SHullShader* shader = new SHullShader();
		CreateShader(aHullShader, shader);
		myHullShaders[aHullShader] = shader;
	}

	void CShaderContainer::LoadDomainShader(const std::string& aDomainShader)
	{
		SDomainShader* shader = new SDomainShader();
		CreateShader(aDomainShader, shader);
		myDomainShaders[aDomainShader] = shader;
	}

	void CShaderContainer::LoadComputeShader(const std::string& aComputeShader)
	{
		SComputeShader* shader = new SComputeShader();
		CreateShader(aComputeShader, shader);
		myComputeShaders[aComputeShader] = shader;
	}

	void CShaderContainer::CreateShader(const std::string& aShader, SVertexShader*& aVertexShader)
	{

		ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

		ENGINE_LOG("Creating vertexshader %s", aShader.c_str()); //Only first letter is being printed to log.
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
			DL_MESSAGE("%s", (char*)compilationMessage->GetBufferPointer());
		}
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &aVertexShader->vertexShader);

		aVertexShader->compiledShader = compiledShader;

		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aVertexShader->vertexShader, "VertexShader");
	}

	void CShaderContainer::CreateShader(const std::string& aShader, SPixelShader*& aPixelShader)
	{
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
			DL_MESSAGE("%s", (char*)compilationMessage->GetBufferPointer());
		}
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &aPixelShader->pixelShader);
		aPixelShader->compiledShader = compiledShader;
		//aPixelShader->compiledShader = compiledShader->GetBufferPointer();
		//aPixelShader->byteLength = compiledShader->GetBufferSize();

		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aPixelShader->pixelShader, "PixelShader");
	}

	void CShaderContainer::CreateShader(const std::string& aShader, SGeometryShader*& aGeometryShader)
	{
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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");
		
		hr = device->CreateGeometryShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &aGeometryShader->geometryShader);
		aGeometryShader->compiledShader = compiledShader;
		//aGeometryShader->compiledShader = compiledShader->GetBufferPointer();
		//aGeometryShader->byteLength = compiledShader->GetBufferSize();
		
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aGeometryShader->geometryShader, "GeometryShader");
	}

	void CShaderContainer::CreateShader(const std::string& aShader, SHullShader*& aHullShader)
	{
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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreateHullShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &aHullShader->hullShader);

		aHullShader->compiledShader = compiledShader;
		//aHullShader->compiledShader = compiledShader->GetBufferPointer();
		//aHullShader->byteLength = compiledShader->GetBufferSize();


		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aHullShader->hullShader, "HullShader");
	}

	void CShaderContainer::CreateShader(const std::string& aShader, SDomainShader*& aDomainShader)
	{
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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreateDomainShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &aDomainShader->domainShader);
		aDomainShader->compiledShader = compiledShader;
		//aDomainShader->compiledShader = compiledShader->GetBufferPointer();
		//aDomainShader->byteLength = compiledShader->GetBufferSize();


		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aDomainShader->domainShader, "DomainShader");
	}

	void CShaderContainer::CreateShader(const std::string& aShader, SComputeShader*& aComputeShader)
	{
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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreateComputeShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &aComputeShader->computeShader);
		aComputeShader->compiledShader = compiledShader;
		//aComputeShader->compiledShader = compiledShader->GetBufferPointer();
		//aComputeShader->byteLength = compiledShader->GetBufferSize();

		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aComputeShader->computeShader, "ComputeShader");
	}

	void CShaderContainer::Create()
	{
		DL_ASSERT_EXP(myInstance == nullptr, "Failed to create ShaderContainer. Instance was not null.");
		myInstance = new CShaderContainer();
	}

	void CShaderContainer::Destroy()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "Failed to destroy ShaderContainer. Instance was null.");
		SAFE_DELETE(myInstance);
	}

	CShaderContainer* CShaderContainer::GetInstance()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "Failed to get ShaderContainer. Instance was null.");
		return myInstance;
	}

	SVertexShader* CShaderContainer::GetVertexShader(const std::string& aVertexShader)
	{
		if (myVertexShaders.find(aVertexShader) == myVertexShaders.end())
		{
			LoadVertexShader(aVertexShader);
		}
		return myVertexShaders[aVertexShader];
	}

	SPixelShader* CShaderContainer::GetPixelShader(const std::string& aPixelShader)
	{
		if (myPixelShaders.find(aPixelShader) == myPixelShaders.end())
		{
			LoadPixelShader(aPixelShader);
		}
		return myPixelShaders[aPixelShader];
	}

	SGeometryShader* CShaderContainer::GetGeometryShader(const std::string& aGeometryShader)
	{
		if (myGeometryShaders.find(aGeometryShader) == myGeometryShaders.end())
		{
			LoadGeometryShader(aGeometryShader);
		}
		return myGeometryShaders[aGeometryShader];
	}

	SHullShader* CShaderContainer::GetHullShader(const std::string& aHullShader)
	{
		if (myHullShaders.find(aHullShader) == myHullShaders.end())
		{
			LoadHullShader(aHullShader);
		}
		return myHullShaders[aHullShader];
	}

	SDomainShader* CShaderContainer::GetDomainShader(const std::string& aDomainShader)
	{
		if (myDomainShaders.find(aDomainShader) == myDomainShaders.end())
		{
			LoadDomainShader(aDomainShader);
		}
		return myDomainShaders[aDomainShader];
	}

	SComputeShader* CShaderContainer::GetComputeShader(const std::string& aComputeShader)
	{
		if (myComputeShaders.find(aComputeShader) == myComputeShaders.end())
		{
			LoadComputeShader(aComputeShader);
		}
		return myComputeShaders[aComputeShader];
	}

};
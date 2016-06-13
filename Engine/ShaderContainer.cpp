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
			SAFE_RELEASE(it->second);
		}

		for (ITTERATE(myPixelShaders))
		{
			SAFE_RELEASE(it->second);
		}

		for (ITTERATE(myGeometryShaders))
		{
			SAFE_RELEASE(it->second);
		}

		for (ITTERATE(myHullShaders))
		{
			SAFE_RELEASE(it->second);
		}

		for (ITTERATE(myDomainShaders))
		{
			SAFE_RELEASE(it->second);
		}

		for (ITTERATE(myComputeShaders))
		{
			SAFE_RELEASE(it->second);
		}
	}

	void CShaderContainer::LoadVertexShader(const std::string& aVertexShader)
	{
		ID3D11VertexShader* shader = nullptr;
		CreateShader(aVertexShader, shader);
		myVertexShaders[aVertexShader] = shader;
	}

	void CShaderContainer::LoadPixelShader(const std::string& aPixelShader)
	{
		ID3D11PixelShader* shader = nullptr;
		CreateShader(aPixelShader, shader);
		myPixelShaders[aPixelShader] = shader;
	}

	void CShaderContainer::LoadGeometryShader(const std::string& aGeometryShader)
	{
		ID3D11GeometryShader* shader = nullptr;
		CreateShader(aGeometryShader, shader);
		myGeometryShaders[aGeometryShader] = shader;
	}

	void CShaderContainer::LoadHullShader(const std::string& aHullShader)
	{
		ID3D11HullShader* shader = nullptr;
		CreateShader(aHullShader, shader);
		myHullShaders[aHullShader] = shader;
	}

	void CShaderContainer::LoadDomainShader(const std::string& aDomainShader)
	{
		ID3D11DomainShader* shader = nullptr;
		CreateShader(aDomainShader, shader);
		myDomainShaders[aDomainShader] = shader;
	}

	void CShaderContainer::LoadComputeShader(const std::string& aComputeShader)
	{
		ID3D11ComputeShader* shader = nullptr;
		CreateShader(aComputeShader, shader);
		myComputeShaders[aComputeShader] = shader;
	}

	void CShaderContainer::CreateShader(const std::string& aShader, ID3D11VertexShader*& aVertexShader)
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

		//D3DReadFileToBlob(fileName.c_str(), &compiledShader);
		hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "VS", "vs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);
		if (compilationMessage != nullptr)
		{
			DL_MESSAGE("%s", (char*)compilationMessage->GetBufferPointer());
		}
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");


		hr = device->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &aVertexShader);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aVertexShader, "VertexShader");
	}

	void CShaderContainer::CreateShader(const std::string& aShader, ID3D11PixelShader*& aPixelShader)
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
		hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, NULL, NULL, shaderFlag, NULL, &compiledShader, &compilationMessage);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreatePixelShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &aPixelShader);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aPixelShader, "PixelShader");
	}

	void CShaderContainer::CreateShader(const std::string& aShader, ID3D11GeometryShader*& aGeometryShader)
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
		hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, NULL, NULL, shaderFlag, NULL, &compiledShader, &compilationMessage);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreateGeometryShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &aGeometryShader);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aGeometryShader, "GeometryShader");
	}

	void CShaderContainer::CreateShader(const std::string& aShader, ID3D11HullShader*& aHullShader)
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
		hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, NULL, NULL, shaderFlag, NULL, &compiledShader, &compilationMessage);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreateHullShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &aHullShader);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aHullShader, "HullShader");
	}

	void CShaderContainer::CreateShader(const std::string& aShader, ID3D11DomainShader*& aDomainShader)
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
		hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, NULL, NULL, shaderFlag, NULL, &compiledShader, &compilationMessage);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreateDomainShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &aDomainShader);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aDomainShader, "DomainShader");
	}

	void CShaderContainer::CreateShader(const std::string& aShader, ID3D11ComputeShader*& aComputeShader)
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
		hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, NULL, NULL, shaderFlag, NULL, &compiledShader, &compilationMessage);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");

		hr = device->CreateComputeShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &aComputeShader);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Vertex Shader.");
		CEngine::GetDirectX()->SetDebugName(aComputeShader, "ComputeShader");
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

	ID3D11VertexShader* CShaderContainer::GetVertexShader(const std::string& aVertexShader)
	{
		if (myVertexShaders.find(aVertexShader) == myVertexShaders.end())
		{
			LoadVertexShader(aVertexShader);
		}
		return myVertexShaders[aVertexShader];
	}

	ID3D11PixelShader* CShaderContainer::GetPixelShader(const std::string& aPixelShader)
	{
		if (myPixelShaders.find(aPixelShader) == myPixelShaders.end())
		{
			LoadPixelShader(aPixelShader);
		}
		return myPixelShaders[aPixelShader];
	}

	ID3D11GeometryShader* CShaderContainer::GetGeometryShader(const std::string& aGeometryShader)
	{
		if (myGeometryShaders.find(aGeometryShader) == myGeometryShaders.end())
		{
			LoadGeometryShader(aGeometryShader);
		}
		return myGeometryShaders[aGeometryShader];
	}

	ID3D11HullShader* CShaderContainer::GetHullShader(const std::string& aHullShader)
	{
		if (myHullShaders.find(aHullShader) == myHullShaders.end())
		{
			LoadHullShader(aHullShader);
		}
		return myHullShaders[aHullShader];
	}

	ID3D11DomainShader* CShaderContainer::GetDomainShader(const std::string& aDomainShader)
	{
		if (myDomainShaders.find(aDomainShader) == myDomainShaders.end())
		{
			LoadDomainShader(aDomainShader);
		}
		return myDomainShaders[aDomainShader];
	}

	ID3D11ComputeShader* CShaderContainer::GetComputeShader(const std::string& aComputeShader)
	{
		if (myComputeShaders.find(aComputeShader) == myComputeShaders.end())
		{
			LoadComputeShader(aComputeShader);
		}
		return myComputeShaders[aComputeShader];
	}

};
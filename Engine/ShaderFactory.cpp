#include "stdafx.h"
#include "ShaderFactory.h"
#include <d3dcompiler.h>
#include <Utilities.h>
#include <JSON/JSONReader.h>
namespace Snowblind
{
	CShaderFactory::CShaderFactory()
	{
	}

	CShaderFactory::~CShaderFactory()
	{
	}

	void CShaderFactory::CreateShader(CEffect* anEffect)
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
			CreateShader(input, anEffect->myVertexShader);
		}

		input.clear();
		input = sub + "/" + pixelShader;
		if (pixelShader != "")
		{
			CreateShader(input, anEffect->myPixelShader);
		}

		input.clear();
		input = sub + "/" + geometryShader;
		if (geometryShader != "")
		{
			CreateShader(input, anEffect->myGeometryShader);
		}

		input.clear();
		input = sub + "/" + hullShader;
		if (hullShader != "")
		{
			CreateShader(input, anEffect->myHullShader);
		}

		input.clear();
		input = sub + "/" + domainShader;
		if (domainShader != "")
		{
			CreateShader(input, anEffect->myDomainShader);
		}

		input.clear();
		input = sub + "/" + computeShader;
		if (computeShader != "")
		{
			CreateShader(input, anEffect->myComputeShader);
		}

	}

	//----------------------------------------
	// Vertex Shader
	//----------------------------------------
	void CShaderFactory::CreateShader(const std::string& aShader, SVertexShader* aVertexShader)
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

	//----------------------------------------
	// Pixel Shader
	//----------------------------------------
	void CShaderFactory::CreateShader(const std::string& aShader, SPixelShader* aPixelShader)
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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Pixel Shader.");
		aPixelShader->compiledShader = compiledShader;

		CEngine::GetDirectX()->SetDebugName(aPixelShader->pixelShader, "PixelShader");
	}

	//----------------------------------------
	// Geometry Shader
	//----------------------------------------
	void CShaderFactory::CreateShader(const std::string& aShader, SGeometryShader* aGeometryShader)
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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Geometry Shader.");
		aGeometryShader->compiledShader = compiledShader;

		CEngine::GetDirectX()->SetDebugName(aGeometryShader->geometryShader, "GeometryShader");
	}

	//----------------------------------------
	// Hull Shader
	//----------------------------------------
	void CShaderFactory::CreateShader(const std::string& aShader, SHullShader* aHullShader)
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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Hull Shader.");
		aHullShader->compiledShader = compiledShader;
		CEngine::GetDirectX()->SetDebugName(aHullShader->hullShader, "HullShader");
	}

	//----------------------------------------
	// Domain Shader
	//----------------------------------------
	void CShaderFactory::CreateShader(const std::string& aShader, SDomainShader* aDomainShader)
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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Domain Shader.");
		aDomainShader->compiledShader = compiledShader;

		CEngine::GetDirectX()->SetDebugName(aDomainShader->domainShader, "DomainShader");
	}

	//----------------------------------------
	// Compute Shader
	//----------------------------------------
	void CShaderFactory::CreateShader(const std::string& aShader, SComputeShader* aComputeShader)
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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Compute Shader.");
		aComputeShader->compiledShader = compiledShader;

		CEngine::GetDirectX()->SetDebugName(aComputeShader->computeShader, "ComputeShader");
	}


};
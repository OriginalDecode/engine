#include "stdafx.h"
#include "ShaderFactory.h"
#include <d3dcompiler.h>
#include <Utilities.h>
#include <JSON/JSONReader.h>
#include "../CommonLib/DataStructures/Hashmap/Hash.h"
#define SLEEP_TIME 1000

namespace Snowblind
{
	ShaderFactory::ShaderFactory()
	{
		for (int i = 0; i < 6; i++)
		{
			FileWatcher* watcher = new FileWatcher();
			myFileWatchers.Insert(i, watcher);
		}
	}

	ShaderFactory::~ShaderFactory()
	{
		myFileWatchers.DeleteAll();
		/*	DELETE_MAP(myVertexShaders);
			DELETE_MAP(myPixelShaders);
			DELETE_MAP(myGeometryShaders);
			DELETE_MAP(myHullShaders);
			DELETE_MAP(myDomainShaders);
			DELETE_MAP(myComputeShaders);*/
	}

	void ShaderFactory::LoadShader(Effect* anEffect)
	{
		std::string path = anEffect->myFileName;
		std::string sub = CL::substr(path, "/", true, 0) + "/";

		JSONReader reader(path);
		/*	
		std::string geometry_shader = reader.ReadElement("GeometryShader");
		std::string hull_shader = reader.ReadElement("HullShader");
		std::string domain_shader = reader.ReadElement("DomainShader");
		std::string compute_shader = reader.ReadElement("ComputeShader");
		*/

		LoadShader(sub + reader.ReadElement("VertexShader"), anEffect);
		LoadShader(sub + reader.ReadElement("PixelShader"), anEffect);

	}

	//________________________________________
	// Test Create
	void ShaderFactory::LoadShader(const std::string& file_path, Effect* effect)
	{

		u64 hash_key = Hash(file_path);

		if (m_Shaders.find(hash_key) == m_Shaders.end())
		{
			m_Shaders[hash_key] = CreateShader(file_path, "VS");
			myFileWatchers[u32(eShaderType::VERTEX)]->WatchFileChangeWithDependencies(file_path
				, std::bind(&ShaderFactory::OnReload, this, std::placeholders::_1));
		}
		//effect->m_VertexShader = m_Shaders[hash_key];
		m_Shaders[hash_key]->effectPointers.Add(effect);
	}

	CompiledShader* ShaderFactory::CreateShader(const std::string& file_path, const std::string& shader_type, const std::string& feature_level /*= "_5_0"*/)
	{
		CompiledShader* new_shader = new CompiledShader;
		IDevice* device = Engine::GetAPI()->GetDevice();

		ENGINE_LOG("Creating %s", file_path.c_str());

		IBlob* compiled_shader = CompileShader(file_path, shader_type, shader_type + feature_level);
		new_shader->m_Shader = Engine::GetInstance()->CreateShader(compiled_shader, shader_type, file_path);

		new_shader->blob = compiled_shader;
		new_shader->compiledShader = compiled_shader->GetBufferPointer();
		new_shader->shaderSize = compiled_shader->GetBufferSize();

		return new_shader;
	}

	void ShaderFactory::OnReload(const std::string& file_path)
	{
		Sleep(SLEEP_TIME);
		u64 hash_key = Hash(file_path);
		m_Shaders[hash_key] = CreateShader(file_path, "VS");
		for (Effect* effect : m_Shaders[hash_key]->effectPointers)
		{
			effect->myVertexShader = m_Shaders[hash_key];
			DL_ASSERT_EXP(effect->myVertexShader != nullptr, "Vertex Shader pointer was null. Something failed on Shader Reload.");
			m_Shaders[hash_key]->effectPointers.Add(effect);
		}
	}

	//________________________________________
	// Vertex Shader
//	bool ShaderFactory::LoadVertexShader(const std::string& file_path, Effect* effect)
//	{
//		if (myVertexShaders.find(file_path) == myVertexShaders.end())
//		{
//			myVertexShaders[file_path] = CreateVertexShader(file_path);
//			if (!myVertexShaders[file_path])
//				return false;
//
//			myFileWatchers[u32(eShaderType::VERTEX)]->WatchFileChangeWithDependencies(file_path
//				, std::bind(&ShaderFactory::ReloadVertex, this, std::placeholders::_1));
//		}
//		effect->myVertexShader = myVertexShaders[file_path];
//		myVertexShaders[file_path]->effectPointers.Add(effect);
//		return true;
//	}
//
//	VertexShader* ShaderFactory::CreateVertexShader(const std::string& file_path)
//	{
//		VertexShader* newShader = new VertexShader();
//#ifdef SNOWBLIND_DX11
//		IDevice* device = Engine::GetAPI()->GetDevice();
//
//		ENGINE_LOG("Creating vertexshader %s", file_path.c_str());
//
//		IBlob* compiled_shader = CompileShader(file_path, "VS", "vs_5_0");
//		IVertexShader* vertex_shader = nullptr;
//		HRESULT hr = device->CreateVertexShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &vertex_shader);
//		newShader->m_Shader = vertex_shader;
//
//
//		newShader->blob = compiled_shader;
//		newShader->compiledShader = compiled_shader->GetBufferPointer();
//		newShader->shaderSize = compiled_shader->GetBufferSize();
//
//		Engine::GetAPI()->HandleErrors(hr, "Failed to Create Vertex Shader.");
//		Engine::GetAPI()->SetDebugName(newShader->vertexShader, file_path);
//#endif
//		return newShader;
//	}
//
//	void ShaderFactory::ReloadVertex(const std::string& aFilePath)
//	{
//		Sleep(SLEEP_TIME);
//		CU::GrowingArray<Effect*> effectPointers = GetEffectArray(aFilePath);
//		myVertexShaders[aFilePath] = CreateVertexShader(aFilePath);
//		for each (Effect* effect in effectPointers)
//		{
//			effect->myVertexShader = myVertexShaders[aFilePath];
//			DL_ASSERT_EXP(effect->myVertexShader != nullptr, "Vertex Shader pointer was null. Something failed on Shader Reload.");
//			myVertexShaders[aFilePath]->effectPointers.Add(effect);
//		}
//	}
//
//	//________________________________________
//	// Pixel Shader
//	void ShaderFactory::LoadPixelShader(const std::string& file_path, Effect* effect)
//	{
//		if (myPixelShaders.find(file_path) == myPixelShaders.end())
//		{
//			myPixelShaders[file_path] = CreatePixelShader(file_path);
//			myFileWatchers[u32(eShaderType::PIXEL)]->WatchFileChangeWithDependencies(file_path, std::bind(&ShaderFactory::ReloadPixel, this,std::placeholders::_1));
//		}
//
//		effect->myPixelShader = myPixelShaders[file_path];
//		myPixelShaders[file_path]->effectPointers.Add(effect);
//	}
//
//	PixelShader* ShaderFactory::CreatePixelShader(const std::string& file_path)
//	{
//		PixelShader* newShader = new PixelShader();
//#ifdef SNOWBLIND_DX11
//		myPixelShaders[file_path] = newShader;
//		IDevice* device = Engine::GetAPI()->GetDevice();
//		ENGINE_LOG("Creating pixelshader : %s", file_path.c_str());
//		
//		HRESULT hr;
//		IBlob* compiled_shader = CompileShader(file_path, "PS", "ps_5_0");
//		DL_ASSERT_EXP(compiled_shader, "Shader was null");
//		hr = device->CreatePixelShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &newShader->pixelShader);
//		Engine::GetAPI()->HandleErrors(hr, "Failed to Create Pixel Shader.");
//
//		newShader->blob = compiled_shader;
//		newShader->compiledShader = compiled_shader->GetBufferPointer();
//		newShader->shaderSize = compiled_shader->GetBufferSize();
//
//		Engine::GetAPI()->SetDebugName(newShader->pixelShader, file_path);
//#endif
//		return newShader;
//	}
//
//	void ShaderFactory::ReloadPixel(const std::string& aFilePath)
//	{
//		Sleep(SLEEP_TIME);
//		CU::GrowingArray<Effect*> effectPointers = GetEffectArray(aFilePath);
//		myPixelShaders[aFilePath] = CreatePixelShader(aFilePath);
//		for each (Effect* effect in effectPointers)
//		{
//			effect->myPixelShader = myPixelShaders[aFilePath];
//			DL_ASSERT_EXP(effect->myPixelShader != nullptr, "Vertex Shader pointer was null. Something failed on Shader Reload.");
//			myPixelShaders[aFilePath]->effectPointers.Add(effect);
//		}
//	}
//
//	//________________________________________
//	// Geometry Shader
//	void ShaderFactory::LoadGeometryShader(const std::string& file_path, Effect* effect)
//	{
//#ifdef SNOWBLIND_DX11
//		if (myGeometryShaders.find(file_path) == myGeometryShaders.end())
//		{
//			GeometryShader* newShader = new GeometryShader();
//			IDevice* device = Engine::GetAPI()->GetDevice();
//
//			ENGINE_LOG("Creating geometryshader : %s", file_path.c_str());
//			HRESULT hr;
//			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
//#ifdef _DEBUG 
//			shaderFlag |= D3D10_SHADER_DEBUG;
//			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
//#endif
//
//			IBlob* compiledShader = 0;
//			IBlob* compilationMessage = 0;
//			std::wstring fileName(file_path.begin(), file_path.end());
//			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "GS", "gs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);
//			if (compilationMessage != nullptr)
//			{
//				std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
//				DL_WARNING("%s", msg.c_str());
//			}
//
//			Engine::GetAPI()->HandleErrors(hr, "Failed to Compile Effect.");
//
//			hr = device->CreateGeometryShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &newShader->geometryShader);
//			Engine::GetAPI()->HandleErrors(hr, "Failed to Create Geometry Shader.");
//			newShader->blob = compiledShader;
//			newShader->compiledShader = compiledShader->GetBufferPointer();
//			newShader->shaderSize = compiledShader->GetBufferSize();
//			Engine::GetAPI()->SetDebugName(newShader->geometryShader, "GeometryShader");
//			myGeometryShaders[file_path] = newShader;
//		}
//		effect->myGeometryShader = myGeometryShaders[file_path];
//#endif
//	}
//
//	//________________________________________
//	// Hull Shader
//	void ShaderFactory::LoadHullShader(const std::string& file_path, Effect* effect)
//	{
//#ifdef SNOWBLIND_DX11
//		if (myHullShaders.find(file_path) == myHullShaders.end())
//		{
//			HullShader* newShader = new HullShader();
//			IDevice* device = Engine::GetAPI()->GetDevice();
//
//			ENGINE_LOG("Creating hullshader : %s", file_path.c_str());
//			HRESULT hr;
//			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
//#ifdef _DEBUG 
//			shaderFlag |= D3D10_SHADER_DEBUG;
//			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
//#endif
//
//			IBlob* compiledShader = 0;
//			IBlob* compilationMessage = 0;
//
//			std::wstring fileName(file_path.begin(), file_path.end());
//			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "HS", "hs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);
//			if (compilationMessage != nullptr)
//			{
//				std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
//				DL_WARNING("%s", msg.c_str());
//			}
//
//			Engine::GetAPI()->HandleErrors(hr, "Failed to Compile Effect.");
//
//			hr = device->CreateHullShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &newShader->hullShader);
//			Engine::GetAPI()->HandleErrors(hr, "Failed to Create Hull Shader.");
//			newShader->blob = compiledShader;
//			newShader->compiledShader = compiledShader->GetBufferPointer();
//			newShader->shaderSize = compiledShader->GetBufferSize();
//			Engine::GetAPI()->SetDebugName(newShader->hullShader, "HullShader");
//			myHullShaders[file_path] = newShader;
//		}
//		effect->myHullShader = myHullShaders[file_path];
//#endif
//	}
//
//	//________________________________________
//	// Domain Shader
//	void ShaderFactory::LoadDomainShader(const std::string& file_path, Effect* effect)
//	{
//
//#ifdef SNOWBLIND_DX11
//		if (myDomainShaders.find(file_path) == myDomainShaders.end())
//		{
//			DomainShader* newShader = new DomainShader();
//			IDevice* device = Engine::GetAPI()->GetDevice();
//
//			ENGINE_LOG("Creating domainshader : %s", file_path.c_str());
//			HRESULT hr;
//			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
//#ifdef _DEBUG 
//			shaderFlag |= D3D10_SHADER_DEBUG;
//			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
//#endif
//
//			IBlob* compiledShader = 0;
//			IBlob* compilationMessage = 0;
//
//			std::wstring fileName(file_path.begin(), file_path.end());
//			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "DS", "ds_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);
//
//			if (compilationMessage != nullptr)
//			{
//				std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
//				DL_WARNING("%s", msg.c_str());
//			}
//
//			Engine::GetAPI()->HandleErrors(hr, "Failed to Compile Effect.");
//
//			hr = device->CreateDomainShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &newShader->domainShader);
//			Engine::GetAPI()->HandleErrors(hr, "Failed to Create Domain Shader.");
//			newShader->blob = compiledShader;
//			newShader->compiledShader = compiledShader->GetBufferPointer();
//			newShader->shaderSize = compiledShader->GetBufferSize();
//			Engine::GetAPI()->SetDebugName(newShader->domainShader, "DomainShader");
//			myDomainShaders[file_path] = newShader;
//		}
//		effect->myDomainShader = myDomainShaders[file_path];
//#endif
//	}
//
//	//________________________________________
//	// Compute Shader
//	void ShaderFactory::LoadComputeShader(const std::string& file_path, Effect* effect)
//	{
//#ifdef SNOWBLIND_DX11
//		if (myComputeShaders.find(file_path) == myComputeShaders.end())
//		{
//			ComputeShader* newShader = new ComputeShader();
//			IDevice* device = Engine::GetAPI()->GetDevice();
//
//			ENGINE_LOG("Creating computeshader : %s", file_path.c_str());
//			HRESULT hr;
//			unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
//#ifdef _DEBUG 
//			shaderFlag |= D3D10_SHADER_DEBUG;
//			shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
//#endif
//
//			IBlob* compiledShader = 0;
//			IBlob* compilationMessage = 0;
//
//			std::wstring fileName(file_path.begin(), file_path.end());
//			hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, "CS", "cs_5_0", shaderFlag, NULL, &compiledShader, &compilationMessage);
//
//			if (compilationMessage != nullptr)
//			{
//				std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
//				DL_WARNING("%s", msg.c_str());
//			}
//
//			Engine::GetAPI()->HandleErrors(hr, "Failed to Compile Effect.");
//
//			hr = device->CreateComputeShader(compiledShader, compiledShader->GetBufferSize(), nullptr, &newShader->computeShader);
//			Engine::GetAPI()->HandleErrors(hr, "Failed to Create Compute Shader.");
//			newShader->blob = compiledShader;
//			newShader->compiledShader = compiledShader->GetBufferPointer();
//			newShader->shaderSize = compiledShader->GetBufferSize();
//			Engine::GetAPI()->SetDebugName(newShader->computeShader, "ComputeShader");
//
//			myComputeShaders[file_path] = newShader;
//		}
//		effect->myComputeShader = myComputeShaders[file_path];
//#endif
//	}
//	


	IBlob* ShaderFactory::CompileShader(const std::string& file_path, const std::string& shader_type, const std::string& feature_level)
	{
		unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
		shaderFlag |= D3D10_SHADER_DEBUG;
		shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
		IBlob* compiledShader = nullptr;
		IBlob* compilationMessage = nullptr;

		//Can this be more generalized?
		Engine::GetInstance()->CompileShaderFromFile(file_path, shader_type, feature_level, shaderFlag, compiledShader, compilationMessage);
		DL_ASSERT_EXP(compiledShader, "Shader was null?");
		if (compilationMessage != nullptr)
		{
			//std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
			DL_WARNING("%s", (char*)compilationMessage->GetBufferPointer());
			// (#LINUS) Should be output to a warninglist in engine debug tools.
			//DL_WARNINGBOX((char*)compilationMessage->GetBufferPointer());
		}
		return compiledShader;
	}

	CU::GrowingArray<Effect*> ShaderFactory::GetEffectArray(const std::string& aFilePath)
	{
		CU::GrowingArray<Effect*> effectPointers;

		//________________________________________
		// Vertex Shader
		if (myVertexShaders.find(aFilePath) != myVertexShaders.end())
		{
			for each(Effect* effect in myVertexShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myVertexShaders[aFilePath];
			myVertexShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		//________________________________________
		// Pixel Shader
		if (myPixelShaders.find(aFilePath) != myPixelShaders.end())
		{
			for each(Effect* effect in myPixelShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myPixelShaders[aFilePath];
			myPixelShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		//________________________________________
		// Geometry Shader
		if (myGeometryShaders.find(aFilePath) != myGeometryShaders.end())
		{
			for each(Effect* effect in myGeometryShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myGeometryShaders[aFilePath];
			myGeometryShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		//________________________________________
		// Hull Shader
		if (myHullShaders.find(aFilePath) != myHullShaders.end())
		{
			for each(Effect* effect in myHullShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myHullShaders[aFilePath];
			myHullShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		//________________________________________
		// Domain Shader
		if (myDomainShaders.find(aFilePath) != myDomainShaders.end())
		{
			for each(Effect* effect in myDomainShaders[aFilePath]->effectPointers)
			{
				effectPointers.Add(effect);
			}
			delete myDomainShaders[aFilePath];
			myDomainShaders[aFilePath] = nullptr;
			return effectPointers;
		}

		//________________________________________
		// Compute Shader
		if (myComputeShaders.find(aFilePath) != myComputeShaders.end())
		{
			for each(Effect* effect in myComputeShaders[aFilePath]->effectPointers)
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

	void ShaderFactory::Update()
	{
		for (FileWatcher* watcher : myFileWatchers)
		{
			watcher->FlushChanges();
		}
	}

	//________________________________________
	// Shader Structs

	CompiledShader::~CompiledShader()
	{
		SAFE_RELEASE(blob);

		IUnknown* unknown_pointer = m_Shader;
		unknown_pointer->Release();
		unknown_pointer = nullptr;
		m_Shader = nullptr;

		compiledShader = nullptr;
	}

	//VertexShader::~VertexShader()
	//{
	//	SAFE_RELEASE(vertexShader);
	//}

	//PixelShader::~PixelShader()
	//{
	//	SAFE_RELEASE(pixelShader);
	//}

	//GeometryShader::~GeometryShader()
	//{
	//	SAFE_RELEASE(geometryShader);
	//}

	//HullShader::~HullShader()
	//{
	//	SAFE_RELEASE(hullShader);
	//}

	//DomainShader::~DomainShader()
	//{
	//	SAFE_RELEASE(domainShader);
	//}

	//ComputeShader::~ComputeShader()
	//{
	//	SAFE_RELEASE(computeShader);
	//}

};
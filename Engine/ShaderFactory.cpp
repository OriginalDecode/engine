#include "stdafx.h"
#include "ShaderFactory.h"
#include <d3dcompiler.h>
#include <Utilities.h>
#include <JSON/JSONReader.h>
#include <DataStructures/Hashmap/Hash.h>
#include <DataStructures/GrowingArray.h>
#define SLEEP_TIME 1000

static constexpr char* vertex_shader = "VS";
static constexpr char* pixel_shader = "PS";
static constexpr char* geometry_shader = "GS";
static constexpr char* hull_shader = "HS";
static constexpr char* domain_shader = "DS";
static constexpr char* compute_shader = "CS";


ShaderFactory::ShaderFactory()
{
#ifndef FINAL 
	for (int i = 0; i < 6; i++)
	{
		FileWatcher* watcher = new FileWatcher();
		myFileWatchers.Insert(i, watcher);
	}
#endif
}

ShaderFactory::~ShaderFactory()
{
#ifndef FINAL 
	myFileWatchers.DeleteAll();
#endif
	DELETE_MAP(m_Shaders);
}

void ShaderFactory::LoadShader(Effect* anEffect)
{
	std::string path = anEffect->myFileName;
	std::string sub = CL::substr(path, "/", true, 0) + "/";

	JSONReader reader(path);

	LoadShader(sub + reader.ReadElement("VertexShader"), anEffect);
	LoadShader(sub + reader.ReadElement("PixelShader"), anEffect);
	LoadShader(sub + reader.ReadElement("GeometryShader"), anEffect);
	LoadShader(sub + reader.ReadElement("HullShader"), anEffect);
	LoadShader(sub + reader.ReadElement("DomainShader"), anEffect);
	LoadShader(sub + reader.ReadElement("ComputeShader"), anEffect);

}


std::string CheckType(const std::string& file_path)
{
	if (file_path.find(vertex_shader) != file_path.npos)
		return vertex_shader;
	else if (file_path.find(pixel_shader) != file_path.npos)
		return pixel_shader;
	else if (file_path.find(geometry_shader) != file_path.npos)
		return geometry_shader;
	else if (file_path.find(hull_shader) != file_path.npos)
		return hull_shader;
	else if (file_path.find(domain_shader) != file_path.npos)
		return domain_shader;
	else if (file_path.find(compute_shader) != file_path.npos)
		return compute_shader;

	return "NO_TYPE";
}

std::string ToLower(const std::string& str)
{
	std::string to_return = str;
	for (s32 i = 0; i < to_return.length(); i++)
	{
		to_return[i] = tolower(str[i]);
	}
	return to_return;
}

void ShaderFactory::LoadShader(const std::string& file_path, Effect* effect)
{
	if (file_path.find(JSONReader::ERROR_STR) != file_path.npos)
		return;


	u64 hash_key = Hash(file_path.c_str());

	if (m_Shaders.find(hash_key) == m_Shaders.end())
	{
		m_Shaders.emplace(hash_key, CreateShader(file_path));
		//m_Shaders[hash_key] = CreateShader(file_path);
	}

	std::string shader_type = CheckType(file_path);

	if (shader_type == vertex_shader)
	{
		effect->m_VertexShader = m_Shaders[hash_key];
#ifndef FINAL 
		myFileWatchers[u32(eShaderType::VERTEX)]->WatchFileChangeWithDependencies(file_path
			, std::bind(&ShaderFactory::OnReload, this, std::placeholders::_1));
#endif
	}
	else if (shader_type == pixel_shader)
	{
		effect->m_PixelShader = m_Shaders[hash_key];
#ifndef FINAL 
		myFileWatchers[u32(eShaderType::PIXEL)]->WatchFileChangeWithDependencies(file_path
			, std::bind(&ShaderFactory::OnReload, this, std::placeholders::_1));
#endif
	}
	else if (shader_type == geometry_shader)
	{
		effect->m_GeometryShader = m_Shaders[hash_key];
#ifndef FINAL 
		myFileWatchers[u32(eShaderType::GEOMETRY)]->WatchFileChangeWithDependencies(file_path
			, std::bind(&ShaderFactory::OnReload, this, std::placeholders::_1));
#endif
	}
	else if (shader_type == hull_shader)
	{
		effect->m_HullShader = m_Shaders[hash_key];
#ifndef FINAL 
		myFileWatchers[u32(eShaderType::HULL)]->WatchFileChangeWithDependencies(file_path
			, std::bind(&ShaderFactory::OnReload, this, std::placeholders::_1));
#endif
	}
	else if (shader_type == domain_shader)
	{
		effect->m_DomainShader = m_Shaders[hash_key];
#ifndef FINAL 
		myFileWatchers[u32(eShaderType::DOMAINS)]->WatchFileChangeWithDependencies(file_path
			, std::bind(&ShaderFactory::OnReload, this, std::placeholders::_1));
#endif
	}
	else if (shader_type == compute_shader)
	{
		effect->m_ComputeShader = m_Shaders[hash_key];
#ifndef FINAL 
		myFileWatchers[u32(eShaderType::COMPUTE)]->WatchFileChangeWithDependencies(file_path
			, std::bind(&ShaderFactory::OnReload, this, std::placeholders::_1));
#endif
	}


	DL_ASSERT_EXP(effect, "Effect pointer was null");
	DL_ASSERT_EXP(m_Shaders[hash_key]->effectPointers.Capacity() > 0, "capacity is bad?");
	m_Shaders[hash_key]->effectPointers.Add(effect);
}

CompiledShader* ShaderFactory::CreateShader(const std::string& file_path, const std::string& feature_level /*= "_5_0"*/)
{
	CompiledShader* new_shader = new CompiledShader;
	IDevice* device = Engine::GetAPI()->GetDevice();

	ENGINE_LOG("Creating %s", file_path.c_str());

	std::string shader_type = CheckType(file_path);
	IBlob* compiled_shader = CompileShader(file_path, shader_type, ToLower(shader_type) + feature_level);
	new_shader->m_Shader = Engine::GetInstance()->CreateShader(compiled_shader, shader_type, file_path);

	new_shader->blob = compiled_shader;
	new_shader->compiledShader = compiled_shader->GetBufferPointer();
	new_shader->shaderSize = compiled_shader->GetBufferSize();

	return new_shader;
}

#ifndef FINAL 
void ShaderFactory::OnReload(const std::string& file_path)
{
	Sleep(SLEEP_TIME);
	u64 hash_key = Hash(file_path.c_str());


	CU::GrowingArray<Effect*> effect_container;
	if (m_Shaders.find(hash_key) != m_Shaders.end())
	{
		const CU::GrowingArray<Effect*>& effects = m_Shaders[hash_key]->effectPointers;
		for (Effect* effect : effects)
		{
			effect_container.Add(effect);
		}

		delete m_Shaders[hash_key];
		m_Shaders[hash_key] = nullptr;
	}

	m_Shaders[hash_key] = CreateShader(file_path);

	for (Effect* effect : effect_container)
	{

		std::string shader_type = CheckType(file_path);

		if (shader_type == vertex_shader)
		{
			effect->m_VertexShader = m_Shaders[hash_key];
			DL_ASSERT_EXP(effect->m_VertexShader != nullptr, "Vertex Shader pointer was null. Something failed on Shader Reload.");
		}
		else if (shader_type == pixel_shader)
		{
			effect->m_PixelShader = m_Shaders[hash_key];
			DL_ASSERT_EXP(effect->m_PixelShader != nullptr, "Pixel Shader pointer was null. Something failed on Shader Reload.");
		}
		else if (shader_type == geometry_shader)
		{
			effect->m_GeometryShader = m_Shaders[hash_key];
			DL_ASSERT_EXP(effect->m_GeometryShader != nullptr, "Geometry Shader pointer was null. Something failed on Shader Reload.");
		}
		else if (shader_type == hull_shader)
		{
			effect->m_HullShader = m_Shaders[hash_key];
			DL_ASSERT_EXP(effect->m_HullShader != nullptr, "Hull Shader pointer was null. Something failed on Shader Reload.");
		}
		else if (shader_type == domain_shader)
		{
			effect->m_DomainShader = m_Shaders[hash_key];
			DL_ASSERT_EXP(effect->m_DomainShader != nullptr, "Domain Shader pointer was null. Something failed on Shader Reload.");
		}
		else if (shader_type == compute_shader)
		{
			effect->m_ComputeShader = m_Shaders[hash_key];
			DL_ASSERT_EXP(effect->m_ComputeShader != nullptr, "Compute Shader pointer was null. Something failed on Shader Reload.");
		}
		m_Shaders[hash_key]->effectPointers.Add(effect);
	}
}
#endif
IBlob* ShaderFactory::CompileShader(const std::string& file_path, const std::string& shader_type, const std::string& feature_level)
{
	unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
	shaderFlag |= D3D10_SHADER_DEBUG;
	shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	IBlob* compiledShader = nullptr;
	IBlob* compilationMessage = nullptr;


	HRESULT hr = Engine::GetInstance()->CompileShaderFromFile(file_path, shader_type, feature_level, shaderFlag, compiledShader, compilationMessage);
	//DL_ASSERT_EXP(compiledShader, "Shader was null?");
	if (compilationMessage != nullptr)
	{
		//std::string msg = myShaderWarningHandler.CheckWarning((char*)compilationMessage->GetBufferPointer(), file_path);
		//DL_WARNING("%s", msg.c_str());
		DL_WARNING("%s has generated warnings!", file_path.c_str())
			DL_WARNING("%s", (char*)compilationMessage->GetBufferPointer());
		// (#LINUS) Should be output to a warninglist in engine debug tools.
		//DL_WARNINGBOX((char*)compilationMessage->GetBufferPointer());
	}
	Engine::GetInstance()->GetAPI()->HandleErrors(hr, "Failed to compile shader!");
	return compiledShader;
}

void ShaderFactory::Update()
{
	for (FileWatcher* watcher : myFileWatchers)
	{
		watcher->FlushChanges();
	}
}

CompiledShader::~CompiledShader()
{
	SAFE_RELEASE(blob);

	IUnknown* unknown_pointer = static_cast<IUnknown*>(m_Shader);
	unknown_pointer->Release();
	unknown_pointer = nullptr;
	m_Shader = nullptr;
	compiledShader = nullptr;
}

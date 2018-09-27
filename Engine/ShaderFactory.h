#pragma once

#include <CommonLib/DataStructures/GrowingArray.h>
#include <CommonLib/DataStructures/StaticArray.h>

#include "engine_shared.h"

#include <map>
#include <Engine/IGraphicsAPI.h>




struct CompiledShader;
class ShaderReload
{
public:
	ShaderReload() = default;
	virtual ~ShaderReload() {};


	virtual void Reload(CompiledShader* shader) = 0;

protected:
};


class FileWatcher;
class Effect;
struct CompiledShader
{
	CompiledShader() = default;
	CompiledShader(IShaderBlob* pShaderBlob, void* pShader, eShaderType shader_type, const char* entrypoint)
		: m_Blob(pShaderBlob)
		, m_Shader(pShader)
		, m_Type(shader_type)
		, m_Entrypoint(entrypoint)
#ifndef _FINAL
#endif
	{
	}

	~CompiledShader();
	void* m_Blob = nullptr;
	void* m_Shader = nullptr;

	eShaderType m_Type;
	std::string m_Entrypoint;

#ifndef _FINAL
	void RegisterReload(ShaderReload* reload)
	{
		m_Reloaders.Add(reload);
	}

	CU::GrowingArray<ShaderReload*> m_Reloaders; //used to rebuild shaders in runtime.
#endif
};

class ShaderFactory
{

public:
	ShaderFactory();
	~ShaderFactory();

	void LoadShader(Effect* anEffect);
	void Update();

	std::map<u64, CompiledShader*> GetCompiledShaders() { return m_Shaders; }
	
	IShaderBlob* CompileShader(const std::string& file_path, const std::string& entrypoint, const std::string& shader_type);

	CompiledShader* GetShader(u64 key);



private:

	void LoadShader(const std::string& filepath, const std::string& entrypoint, const std::string& sampler, eShaderType type, Effect* effect);
	CompiledShader* CreateShader(const std::string& file_path, const std::string& entrypoint, eShaderType type);
	std::map<u64, CompiledShader*> m_Shaders;
#ifndef FINAL 
	CU::StaticArray<FileWatcher*, eShaderType::NOF_TYPES> myFileWatchers;
	void OnReload(const std::string& file_path, const std::string& entrypoint);
#endif

};

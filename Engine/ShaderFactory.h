#pragma once

#include <CommonLib/DataStructures/GrowingArray.h>
#include <CommonLib/DataStructures/StaticArray.h>

#include "engine_shared.h"

#include <map>
#include <Engine/IGraphicsAPI.h>

class FileWatcher;
class Effect;
struct CompiledShader
{
	CompiledShader() = default;
	CompiledShader(IShaderBlob* pShaderBlob, void* pShader, eShaderType shader_type, const char* entrypoint, graphics::eSamplerStates sampler)
		: m_Blob(pShaderBlob)
		, m_Shader(pShader)
		, m_Type(shader_type)
		, m_Entrypoint(entrypoint)
		, m_SamplerState(sampler)
	{
	}

	~CompiledShader();
	void* m_Blob = nullptr;
	void* m_Shader = nullptr;
	eShaderType m_Type;
	std::string m_Entrypoint;
	graphics::eSamplerStates m_SamplerState; //maybe?

#ifndef _FINAL
	CU::GrowingArray<Effect*> m_EffectPointers; //used to rebuild shaders in runtime.
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
private:

	void LoadShader(const std::string& filepath, const std::string& entrypoint, const std::string& sampler, eShaderType type, Effect* effect);
	CompiledShader* CreateShader(const std::string& file_path, const std::string& entrypoint, eShaderType type);
	std::map<u64, CompiledShader*> m_Shaders;
#ifndef FINAL 
	CU::StaticArray<FileWatcher*, (s32)eShaderType::_COUNT> myFileWatchers;
	void OnReload(const std::string& file_path, const std::string& entrypoint);
#endif

};

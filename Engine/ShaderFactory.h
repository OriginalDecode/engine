#pragma once

#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../CommonLib/DataStructures/StaticArray.h"

#include "engine_shared.h"
#include <Engine/IGraphicsAPI.h>

#include <map>

class ShaderWarningHandler;
class FileWatcher;
class Effect;

struct CompiledShader
{
	CompiledShader() = default;
	~CompiledShader();
	void* m_Blob = nullptr;
	void* m_CompiledShader = nullptr;
	void* m_Shader = nullptr;
	s32 shaderSize = 0;
	eShaderType m_Type;
	std::string m_Entrypoint;
	graphics::eSamplerStates m_SamplerState;

#ifndef FINAL
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
	void* CompileShader(const std::string& file_path, const std::string& entrypoint, const std::string& feature_level);
private:

	void LoadShader(const std::string& filepath, const std::string& entrypoint, const std::string& sampler, eShaderType type, Effect* effect);
	CompiledShader* CreateShader(const std::string& file_path, const std::string& entrypoint, eShaderType type, const std::string& feature_level = "_5_0");
	std::map<u64, CompiledShader*> m_Shaders;
#ifndef FINAL 
	CU::StaticArray<FileWatcher*, (s32)eShaderType::_COUNT> myFileWatchers;
	void OnReload(const std::string& file_path, const std::string& entrypoint);
#endif

};

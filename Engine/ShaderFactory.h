#pragma once
#include "ShaderWarningHandler.h"

#include <map>

#include "snowblind_shared.h"

class ShaderWarningHandler;
class FileWatcher;


struct CompiledShader
{
	CompiledShader() = default;
	~CompiledShader();
	IBlob* blob = nullptr;
	void* compiledShader = nullptr;
	void* m_Shader = nullptr;
	int shaderSize = 0;
	CU::GrowingArray<Effect*> effectPointers;
};


class Effect;
class ShaderFactory
{

public:
	ShaderFactory();
	~ShaderFactory();

	void LoadShader(Effect* anEffect);
	void Update();

	std::map<u64, CompiledShader*> GetCompiledShaders() { return m_Shaders; }
	IBlob* CompileShader(const std::string& file_path, const std::string& entrypoint, const std::string& feature_level);
private:

	void LoadShader(const std::string& file_path, Effect* effect);
	CompiledShader* CreateShader(const std::string& file_path, const std::string& feature_level = "_5_0");
	std::map<u64, CompiledShader*> m_Shaders;
#ifndef FINAL 
	CU::StaticArray<FileWatcher*, static_cast<u32>(eShaderType::_COUNT)> myFileWatchers;
	void OnReload(const std::string& file_path);
#endif
	ShaderWarningHandler myShaderWarningHandler;

};

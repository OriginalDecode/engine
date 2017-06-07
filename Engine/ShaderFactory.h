#pragma once

#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../CommonLib/DataStructures/StaticArray.h"
#include "engine_shared.h"

#include <map>

class ShaderWarningHandler;
class FileWatcher;
class Effect;

struct ID3D10Blob;
struct CompiledShader
{
	CompiledShader() = default;
	~CompiledShader();
	ID3D10Blob* blob = nullptr;
	void* compiledShader = nullptr;
	void* m_Shader = nullptr;
	int shaderSize = 0;
	CU::GrowingArray<Effect*> effectPointers; //used to rebuild shaders in runtime.
};

class ShaderFactory
{

public:
	ShaderFactory();
	~ShaderFactory();

	void LoadShader(Effect* anEffect);
	void Update();

	std::map<u64, CompiledShader*> GetCompiledShaders() { return m_Shaders; }
	ID3D10Blob* CompileShader(const std::string& file_path, const std::string& entrypoint, const std::string& feature_level);
private:

	void LoadShader(const std::string& file_path, Effect* effect);
	CompiledShader* CreateShader(const std::string& file_path, const std::string& feature_level = "_5_0");
	std::map<u64, CompiledShader*> m_Shaders;
#ifndef FINAL 
	CU::StaticArray<FileWatcher*, static_cast<u32>(eShaderType::_COUNT)> myFileWatchers;
	void OnReload(const std::string& file_path);
#endif
	//ShaderWarningHandler myShaderWarningHandler;

};

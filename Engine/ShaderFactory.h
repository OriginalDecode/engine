#pragma once
#include "ShaderWarningHandler.h"

#include <map>

#include "snowblind_shared.h"

class ShaderWarningHandler;
class FileWatcher;

namespace Snowblind
{
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
};

namespace Snowblind
{
	class Effect;
	class ShaderFactory
	{

	public:
		ShaderFactory();
		~ShaderFactory();
		
		void LoadShader(Effect* anEffect);
		void Update();

		std::map<u64, CompiledShader*> GetCompiledShaders() { return m_Shaders; }
	private:

		IBlob* CompileShader(const std::string& file_path, const std::string& shader_type, const std::string& feature_level);
		void LoadShader(const std::string& file_path, Effect* effect);
		CompiledShader* CreateShader(const std::string& file_path, const std::string& feature_level = "_5_0");
		void OnReload(const std::string& file_path);
		std::map<u64, CompiledShader*> m_Shaders;
		CU::StaticArray<FileWatcher*, static_cast<u32>(eShaderType::_COUNT)> myFileWatchers;
		ShaderWarningHandler myShaderWarningHandler;
		
	};
};
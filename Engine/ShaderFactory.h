#pragma once
#include <unordered_map>
#include "ShaderWarningHandler.h"

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
	private:

		IBlob* CompileShader(const std::string& file_path, const std::string& shader_type, const std::string& feature_level);

	/*	bool LoadVertexShader(const std::string& file_path, Effect* effect);
		void LoadPixelShader(const std::string& file_path, Effect* effect);
		void LoadGeometryShader(const std::string& file_path, Effect* effect);
		void LoadHullShader(const std::string& file_path, Effect* effect);
		void LoadDomainShader(const std::string& file_path, Effect* effect);
		void LoadComputeShader(const std::string& file_path, Effect* effect);*/

		void LoadShader(const std::string& file_path, Effect* effect);
		CompiledShader* CreateShader(const std::string& file_path, const std::string& shader_type, const std::string& feature_level = "_5_0");
		void OnReload(u64 file_hash);


		/*VertexShader*		CreateVertexShader(const std::string& file_path);
		PixelShader*		CreatePixelShader(const std::string& file_path);
		GeometryShader*		CreateGeometryShader(const std::string& file_path);
		HullShader*			CreateHullShader(const std::string& file_path);
		DomainShader*		CreateDomainShader(const std::string& file_path);
		ComputeShader*		CreateComputeShader(const std::string& file_path);

		void ReloadVertex(const std::string& aFilePath);
		void ReloadPixel(const std::string& aFilePath);
		void ReloadGeometry(const std::string& aFilePath);
		void ReloadHull(const std::string& aFilePath);
		void ReloadDomain(const std::string& aFilePath);
		void ReloadCompute(const std::string& aFilePath);
*/
		std::map<u64, CompiledShader*> m_Shaders;
		/*
				std::unordered_map<std::string, VertexShader*> myVertexShaders;
				std::unordered_map<std::string, PixelShader*> myPixelShaders;
				std::unordered_map<std::string, GeometryShader*> myGeometryShaders;
				std::unordered_map<std::string, HullShader*> myHullShaders;
				std::unordered_map<std::string, DomainShader*> myDomainShaders;
				std::unordered_map<std::string, ComputeShader*> myComputeShaders;
		*/
		
		CU::StaticArray<FileWatcher*, static_cast<u32>(eShaderType::_COUNT)> myFileWatchers;
		ShaderWarningHandler myShaderWarningHandler;
		CU::GrowingArray<Effect*> GetEffectArray(const std::string& aFilePath);
	};
};
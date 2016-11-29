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
		virtual ~CompiledShader();
		IBlob* blob = nullptr;
		void* compiledShader = nullptr;
		int shaderSize = 0;
		CU::GrowingArray<Effect*> effectPointers;
	};

	struct VertexShader : public CompiledShader
	{
		~VertexShader();
		IVertexShader* vertexShader = nullptr;
	};

	struct PixelShader : public CompiledShader
	{
		~PixelShader();
		IPixelShader* pixelShader = nullptr;
	};

	struct GeometryShader : public CompiledShader
	{
		~GeometryShader();
		IGeometryShader* geometryShader = nullptr;
	};

	struct HullShader : public CompiledShader
	{
		~HullShader();
		IHullShader* hullShader = nullptr;
	};

	struct DomainShader : public CompiledShader
	{
		~DomainShader();
		IDomainShader* domainShader = nullptr;
	};

	struct ComputeShader : public CompiledShader
	{
		~ComputeShader();
		IComputeShader* computeShader = nullptr;
	};
}

namespace Snowblind
{
	class Effect;
	class CShaderFactory
	{
	public:
		CShaderFactory();
		~CShaderFactory();
		
		void LoadShader(Effect* anEffect);
		void Update();
	private:

		IBlob* CompileShader(const std::string& file_path, const std::string& shader_type, const std::string& feature_level);

		bool LoadVertexShader(const std::string& file_path, Effect* effect);
		void LoadPixelShader(const std::string& file_path, Effect* effect);
		void LoadGeometryShader(const std::string& file_path, Effect* effect);
		void LoadHullShader(const std::string& file_path, Effect* effect);
		void LoadDomainShader(const std::string& file_path, Effect* effect);
		void LoadComputeShader(const std::string& file_path, Effect* effect);

		VertexShader*		CreateVertexShader(const std::string& file_path);
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

		std::unordered_map<std::string, VertexShader*> myVertexShaders;
		std::unordered_map<std::string, PixelShader*> myPixelShaders;
		std::unordered_map<std::string, GeometryShader*> myGeometryShaders;
		std::unordered_map<std::string, HullShader*> myHullShaders;
		std::unordered_map<std::string, DomainShader*> myDomainShaders;
		std::unordered_map<std::string, ComputeShader*> myComputeShaders;

		
		CU::StaticArray<FileWatcher*, static_cast<u32>(eShaderType::_COUNT)> myFileWatchers;
		ShaderWarningHandler myShaderWarningHandler;
		CU::GrowingArray<Effect*> GetEffectArray(const std::string& aFilePath);
	};
};
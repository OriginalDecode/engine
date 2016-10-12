#pragma once
#include <unordered_map>
#include "ShaderWarningHandler.h"

#ifdef SNOWBLIND_DX11
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11ComputeShader;
struct ID3D10Blob;
#endif
class ShaderWarningHandler;

class FileWatcher;
namespace Snowblind
{

	struct SCompiledShader
	{
		SCompiledShader() {};
		virtual ~SCompiledShader();
#ifdef SNOWBLIND_DX11
		ID3D10Blob* blob = nullptr;
#endif
		void* compiledShader = nullptr;
		int shaderSize = 0;
		CU::GrowingArray<CEffect*> effectPointers;
	};

	struct SVertexShader : public SCompiledShader
	{
		~SVertexShader();
#ifdef SNOWBLIND_DX11
		ID3D11VertexShader* vertexShader = nullptr;
#endif
	};

	struct SPixelShader : public SCompiledShader
	{
		~SPixelShader();
#ifdef SNOWBLIND_DX11
		ID3D11PixelShader* pixelShader = nullptr;
#endif
	};

	struct SGeometryShader : public SCompiledShader
	{
		~SGeometryShader();
#ifdef SNOWBLIND_DX11
		ID3D11GeometryShader* geometryShader = nullptr;
#endif
	};

	struct SHullShader : public SCompiledShader
	{
		~SHullShader();
#ifdef SNOWBLIND_DX11
		ID3D11HullShader* hullShader = nullptr;
#endif
	};

	struct SDomainShader : public SCompiledShader
	{
		~SDomainShader();
#ifdef SNOWBLIND_DX11
		ID3D11DomainShader* domainShader = nullptr;
#endif
	};

	struct SComputeShader : public SCompiledShader
	{
		~SComputeShader();
#ifdef SNOWBLIND_DX11
		ID3D11ComputeShader* computeShader = nullptr;
#endif
	};
}

namespace Snowblind
{
	class CEffect;
	class CShaderFactory
	{
	public:
		CShaderFactory();
		~CShaderFactory();
		
		void LoadShader(CEffect* anEffect);
		void Update();
	private:
#ifdef SNOWBLIND_DX11
		ID3D10Blob* CompileShader(const std::string& file_path, const std::string& shader_type, const std::string& feature_level);
#endif
		bool LoadVertexShader(const std::string& file_path, CEffect* effect);
		void LoadPixelShader(const std::string& file_path, CEffect* effect);
		void LoadGeometryShader(const std::string& file_path, CEffect* effect);
		void LoadHullShader(const std::string& file_path, CEffect* effect);
		void LoadDomainShader(const std::string& file_path, CEffect* effect);
		void LoadComputeShader(const std::string& file_path, CEffect* effect);

		SVertexShader* CreateVertexShader(const std::string& file_path);
		SPixelShader* CreatePixelShader(const std::string& file_path);
		//Missing create Shader functions

		void ReloadVertex(const std::string& aFilePath);
		void ReloadPixel(const std::string& aFilePath);
		void ReloadGeometry(const std::string& aFilePath);
		void ReloadHull(const std::string& aFilePath);
		void ReloadDomain(const std::string& aFilePath);
		void ReloadCompute(const std::string& aFilePath);

		std::unordered_map<std::string, SVertexShader*> myVertexShaders;
		std::unordered_map<std::string, SPixelShader*> myPixelShaders;
		std::unordered_map<std::string, SGeometryShader*> myGeometryShaders;
		std::unordered_map<std::string, SHullShader*> myHullShaders;
		std::unordered_map<std::string, SDomainShader*> myDomainShaders;
		std::unordered_map<std::string, SComputeShader*> myComputeShaders;

		enum class eShaderType
		{
			VERTEX,
			PIXEL,
			GEOMETRY,
			HULL,
			DOMAINS,
			COMPUTE,
			_COUNT
		};
		CU::StaticArray<FileWatcher*, static_cast<u32>(eShaderType::_COUNT)> myFileWatchers;
		ShaderWarningHandler myShaderWarningHandler;
		CU::GrowingArray<CEffect*> GetEffectArray(const std::string& aFilePath);
	};
};
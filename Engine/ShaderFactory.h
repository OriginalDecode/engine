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


typedef ID3D11VertexShader		*pVertexShader;
typedef ID3D11PixelShader		*pPixelShader;
typedef ID3D11GeometryShader	*pGeometryShader;
typedef ID3D11HullShader		*pHullShader;
typedef ID3D11DomainShader		*pDomainShader;
typedef ID3D11ComputeShader		*pComputeShader;
typedef ID3D10Blob				*pBlob;

#else


#endif
class ShaderWarningHandler;

class FileWatcher;
namespace Snowblind
{

	struct SCompiledShader
	{
		SCompiledShader() = default;
		virtual ~SCompiledShader();
		pBlob blob = nullptr;
		void* compiledShader = nullptr;
		int shaderSize = 0;
		CU::GrowingArray<CEffect*> effectPointers;
	};

	struct SVertexShader : public SCompiledShader
	{
		~SVertexShader();
		pVertexShader vertexShader = nullptr;
	};

	struct SPixelShader : public SCompiledShader
	{
		~SPixelShader();
		pPixelShader pixelShader = nullptr;
	};

	struct SGeometryShader : public SCompiledShader
	{
		~SGeometryShader();
		pGeometryShader geometryShader = nullptr;
	};

	struct SHullShader : public SCompiledShader
	{
		~SHullShader();
		pHullShader hullShader = nullptr;
	};

	struct SDomainShader : public SCompiledShader
	{
		~SDomainShader();
		pDomainShader domainShader = nullptr;
	};

	struct SComputeShader : public SCompiledShader
	{
		~SComputeShader();
		pComputeShader computeShader = nullptr;
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

		pBlob CompileShader(const std::string& file_path, const std::string& shader_type, const std::string& feature_level);

		bool LoadVertexShader(const std::string& file_path, CEffect* effect);
		void LoadPixelShader(const std::string& file_path, CEffect* effect);
		void LoadGeometryShader(const std::string& file_path, CEffect* effect);
		void LoadHullShader(const std::string& file_path, CEffect* effect);
		void LoadDomainShader(const std::string& file_path, CEffect* effect);
		void LoadComputeShader(const std::string& file_path, CEffect* effect);

		SVertexShader*		CreateVertexShader(const std::string& file_path);
		SPixelShader*		CreatePixelShader(const std::string& file_path);
		SGeometryShader*	CreateGeometryShader(const std::string& file_path);
		SHullShader*		CreateHullShader(const std::string& file_path);
		SDomainShader*		CreateDomainShader(const std::string& file_path);
		SComputeShader*		CreateComputeShader(const std::string& file_path);

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
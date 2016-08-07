#pragma once
#include <unordered_map>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11ComputeShader;
struct ID3D10Blob;

class ShaderWarningHandler;

class FileWatcher;
namespace Snowblind
{

	struct SCompiledShader
	{
		SCompiledShader() {};
		virtual ~SCompiledShader();
		ID3D10Blob* blob = nullptr;
		void* compiledShader = nullptr;
		int shaderSize = 0;
		CU::GrowingArray<CEffect*> effectPointers;
	};

	struct SVertexShader : public SCompiledShader
	{
		~SVertexShader();
		ID3D11VertexShader* vertexShader = nullptr;
	};

	struct SPixelShader : public SCompiledShader
	{
		~SPixelShader();
		ID3D11PixelShader* pixelShader = nullptr;
	};

	struct SGeometryShader : public SCompiledShader
	{
		~SGeometryShader();
		ID3D11GeometryShader* geometryShader = nullptr;
	};

	struct SHullShader : public SCompiledShader
	{
		~SHullShader();
		ID3D11HullShader* hullShader = nullptr;
	};

	struct SDomainShader : public SCompiledShader
	{
		~SDomainShader();
		ID3D11DomainShader* domainShader = nullptr;
	};

	struct SComputeShader : public SCompiledShader
	{
		~SComputeShader();
		ID3D11ComputeShader* computeShader = nullptr;
	};


	struct SVertexShader;
	struct SPixelShader;
	struct SGeometryShader;
	struct SHullShader;
	struct SDomainShader;
	struct SComputeShader;
	class CEffect;
	class CShaderFactory
	{
	public:
		CShaderFactory();
		~CShaderFactory();
		
		void LoadShader(CEffect* anEffect);
		void Update();
	private:

		void LoadShader(const std::string& aShader, SVertexShader*& aVertexShader);
		void LoadShader(const std::string& aShader, SPixelShader*& aPixelShader);
		void LoadShader(const std::string& aShader, SGeometryShader*& aGeometryShader);
		void LoadShader(const std::string& aShader, SHullShader*& aHullShader);
		void LoadShader(const std::string& aShader, SDomainShader*& aDomainShader);
		void LoadShader(const std::string& aShader, SComputeShader*& aComputeShader);

		void CreateVertexShader(const std::string& aShader);
		void CreatePixelShader(const std::string& aShader);


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

#if defined (_DEBUG)
		CU::GrowingArray<FileWatcher*> myFileWatchers;
		ShaderWarningHandler* myShaderWarningHandler;
#endif
		CU::GrowingArray<CEffect*> GetEffectArray(const std::string& aFilePath);

	};
};
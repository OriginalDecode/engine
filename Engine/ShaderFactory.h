#pragma once
#include <unordered_map>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11ComputeShader;

namespace Snowblind
{

	struct SCompiledShader
	{
		SCompiledShader() {};
		virtual ~SCompiledShader() {};
		void* compiledShader;
		int shaderSize;
	};

	struct SVertexShader : public SCompiledShader
	{
		~SVertexShader() { SAFE_RELEASE(vertexShader); }
		ID3D11VertexShader* vertexShader;
	};

	struct SPixelShader : public SCompiledShader
	{
		~SPixelShader() { SAFE_RELEASE(pixelShader); }
		ID3D11PixelShader* pixelShader;
	};

	struct SGeometryShader : public SCompiledShader
	{
		~SGeometryShader() { SAFE_RELEASE(geometryShader); }
		ID3D11GeometryShader* geometryShader;
	};

	struct SHullShader : public SCompiledShader
	{
		~SHullShader() { SAFE_RELEASE(hullShader); }
		ID3D11HullShader* hullShader;
	};

	struct SDomainShader : public SCompiledShader
	{
		~SDomainShader() { SAFE_RELEASE(domainShader); }
		ID3D11DomainShader* domainShader;
	};

	struct SComputeShader : public SCompiledShader
	{
		~SComputeShader() { SAFE_RELEASE(computeShader); }
		ID3D11ComputeShader* computeShader;
	};


	struct SVertexShader;
	struct SPixelShader;
	struct SGeometryShader;
	struct SHullShader;
	struct SDomainShader;
	struct SComputeShader;
	struct CEffect;
	class CShaderFactory
	{
	public:
		CShaderFactory();
		~CShaderFactory();
		
		void CreateShader(CEffect* anEffect);

	private:
		void CreateShader(const std::string& aShader, SVertexShader*& aVertexShader);
		void CreateShader(const std::string& aShader, SPixelShader*& aPixelShader);
		void CreateShader(const std::string& aShader, SGeometryShader*& aGeometryShader);
		void CreateShader(const std::string& aShader, SHullShader*& aHullShader);
		void CreateShader(const std::string& aShader, SDomainShader*& aDomainShader);
		void CreateShader(const std::string& aShader, SComputeShader*& aComputeShader);

		std::unordered_map<std::string, SVertexShader*> myVertexShaders;
		std::unordered_map<std::string, SPixelShader*> myPixelShaders;
		std::unordered_map<std::string, SGeometryShader*> myGeometryShaders;
		std::unordered_map<std::string, SHullShader*> myHullShaders;
		std::unordered_map<std::string, SDomainShader*> myDomainShaders;
		std::unordered_map<std::string, SComputeShader*> myComputeShaders;



	};
};
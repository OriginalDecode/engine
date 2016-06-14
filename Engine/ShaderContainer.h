#pragma once
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11ComputeShader;

namespace Snowblind
{
	struct SShaderBase
	{
		SShaderBase() {};
		virtual ~SShaderBase() {};
		ID3DBlob* compiledShader;
		
	};

	struct SVertexShader : public SShaderBase
	{
		~SVertexShader() { SAFE_RELEASE(vertexShader); }
		ID3D11VertexShader* vertexShader;
	};

	struct SPixelShader : public SShaderBase
	{
		~SPixelShader() { SAFE_RELEASE(pixelShader); }
		ID3D11PixelShader* pixelShader;
	};

	struct SGeometryShader : public SShaderBase
	{
		~SGeometryShader() { SAFE_RELEASE(geometryShader); }
		ID3D11GeometryShader* geometryShader;
	};

	struct SHullShader : public SShaderBase
	{
		~SHullShader() { SAFE_RELEASE(hullShader); }
		ID3D11HullShader* hullShader;
	};

	struct SDomainShader : public SShaderBase
	{
		~SDomainShader() { SAFE_RELEASE(domainShader); }
		ID3D11DomainShader* domainShader;
	};

	struct SComputeShader : public SShaderBase
	{
		~SComputeShader() { SAFE_RELEASE(computeShader); }
		ID3D11ComputeShader* computeShader;
	};

	class CShaderContainer
	{
	public:
		static void Create();
		static void Destroy();
		static CShaderContainer* GetInstance();

		SVertexShader*		GetVertexShader(const std::string& aVertexShader);
		SPixelShader*		GetPixelShader(const std::string& aPixelShader);
		SGeometryShader*	GetGeometryShader(const std::string& aGeometryShader);
		SHullShader*		GetHullShader(const std::string& aHullShader);
		SDomainShader*		GetDomainShader(const std::string& aDomainShader);
		SComputeShader*		GetComputeShader(const std::string& aComputeShader);

	private:
		static CShaderContainer* myInstance;
		CShaderContainer();
		~CShaderContainer();

		void LoadVertexShader(const std::string& aVertexShader);
		void LoadPixelShader(const std::string& aPixelShader);
		void LoadGeometryShader(const std::string& aGeometryShader);
		void LoadHullShader(const std::string& aHullShader);
		void LoadDomainShader(const std::string& aDomainShader);
		void LoadComputeShader(const std::string& aComputeShader);

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
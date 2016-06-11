#pragma once
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11ComputeShader;

namespace Snowblind
{
	class CShaderContainer
	{
	public:
		static void Create();
		static void Destroy();
		static CShaderContainer* GetInstance();

		ID3D11VertexShader*		GetVertexShader(const std::string& aVertexShader);
		ID3D11PixelShader*		GetPixelShader(const std::string& aPixelShader);
		ID3D11GeometryShader*	GetGeometryShader(const std::string& aGeometryShader);
		ID3D11HullShader*		GetHullShader(const std::string& aHullShader);
		ID3D11DomainShader*		GetDomainShader(const std::string& aDomainShader);
		ID3D11ComputeShader*	GetComputeShader(const std::string& aComputeShader);

	private:
		static CShaderContainer* myInstance;
		CShaderContainer();
		~CShaderContainer();

		ID3D11VertexShader*		LoadVertexShader(const std::string& aVertexShader);
		ID3D11PixelShader*		LoadPixelShader(const std::string& aPixelShader);
		ID3D11GeometryShader*	LoadGeometryShader(const std::string& aGeometryShader);
		ID3D11HullShader*		LoadHullShader(const std::string& aHullShader);
		ID3D11DomainShader*		LoadDomainShader(const std::string& aDomainShader);
		ID3D11ComputeShader*	LoadComputeShader(const std::string& aComputeShader);

		void CreateShader(const std::string& aShader, ID3D11VertexShader* aVertexShader);
		void CreateShader(const std::string& aShader, ID3D11PixelShader* aPixelShader);
		void CreateShader(const std::string& aShader, ID3D11GeometryShader* aGeometryShader);
		void CreateShader(const std::string& aShader, ID3D11HullShader* aHullShader);
		void CreateShader(const std::string& aShader, ID3D11DomainShader* aDomainShader);
		void CreateShader(const std::string& aShader, ID3D11ComputeShader* aComputeShader);

		std::unordered_map<std::string, ID3D11VertexShader*> myVertexShaders;
		std::unordered_map<std::string, ID3D11PixelShader*> myPixelShaders;
		std::unordered_map<std::string, ID3D11GeometryShader*> myGeometryShaders;
		std::unordered_map<std::string, ID3D11HullShader*> myHullShaders;
		std::unordered_map<std::string, ID3D11DomainShader*> myDomainShaders;
		std::unordered_map<std::string, ID3D11ComputeShader*> myComputeShaders;

	};
};
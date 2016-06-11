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

		ID3D11VertexShader*		GetVertexShader(const std::wstring& aVertexShader);
		ID3D11PixelShader*		GetPixelShader(const std::wstring& aPixelShader);
		ID3D11GeometryShader*	GetGeometryShader(const std::wstring& aGeometryShader);
		ID3D11HullShader*		GetHullShader(const std::wstring& aHullShader);
		ID3D11DomainShader*		GetDomainShader(const std::wstring& aDomainShader);
		ID3D11ComputeShader*	GetComputeShader(const std::wstring& aComputeShader);

	private:
		static CShaderContainer* myInstance;
		CShaderContainer();
		~CShaderContainer();

		ID3D11VertexShader*		LoadVertexShader(const std::wstring& aVertexShader);
		ID3D11PixelShader*		LoadPixelShader(const std::wstring& aPixelShader);
		ID3D11GeometryShader*	LoadGeometryShader(const std::wstring& aGeometryShader);
		ID3D11HullShader*		LoadHullShader(const std::wstring& aHullShader);
		ID3D11DomainShader*		LoadDomainShader(const std::wstring& aDomainShader);
		ID3D11ComputeShader*	LoadComputeShader(const std::wstring& aComputeShader);

		void CreateShader(const std::wstring& aShader, ID3D11VertexShader* aVertexShader);
		void CreateShader(const std::wstring& aShader, ID3D11PixelShader* aPixelShader);
		void CreateShader(const std::wstring& aShader, ID3D11GeometryShader* aGeometryShader);
		void CreateShader(const std::wstring& aShader, ID3D11HullShader* aHullShader);
		void CreateShader(const std::wstring& aShader, ID3D11DomainShader* aDomainShader);
		void CreateShader(const std::wstring& aShader, ID3D11ComputeShader* aComputeShader);

		std::unordered_map<std::wstring, ID3D11VertexShader*> myVertexShaders;
		std::unordered_map<std::wstring, ID3D11PixelShader*> myPixelShaders;
		std::unordered_map<std::wstring, ID3D11GeometryShader*> myGeometryShaders;
		std::unordered_map<std::wstring, ID3D11HullShader*> myHullShaders;
		std::unordered_map<std::wstring, ID3D11DomainShader*> myDomainShaders;
		std::unordered_map<std::wstring, ID3D11ComputeShader*> myComputeShaders;

	};
};
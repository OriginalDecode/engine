#pragma once
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

namespace Snowblind
{

	struct VertexShader;
	struct PixelShader;
	struct GeometryShader;
	struct HullShader;
	struct DomainShader;
	struct ComputeShader;
	
	class CTexture;
	class CEffect
	{
		friend class CShaderFactory;
	public:
		CEffect(const std::string& aFilePath);
		~CEffect();
		

		VertexShader* GetVertexShader();
		PixelShader* GetPixelShader();

		/* TODO : IMPLEMENT THE FOLLOWING SHADERS */
		GeometryShader* GetGeometryShader();
		HullShader* GetHullShader();
		DomainShader* GetDomainShader();
		ComputeShader* GetComputeShader();

		void Activate();
		void Deactivate();
		void AddShaderResource(ID3D11ShaderResourceView* aShaderResource);
	private:
		std::string myFileName;
		ID3D11DeviceContext* myContext;

		VertexShader* myVertexShader = nullptr;
		PixelShader* myPixelShader = nullptr;
		GeometryShader* myGeometryShader = nullptr;
		HullShader* myHullShader = nullptr;
		DomainShader* myDomainShader = nullptr;
		ComputeShader* myComputeShader = nullptr;
		
		CU::GrowingArray<ID3D11ShaderResourceView*> myShaderResources;
		CU::GrowingArray<ID3D11ShaderResourceView*> myNULLList;
		bool firstOptimize = false;
	};
};
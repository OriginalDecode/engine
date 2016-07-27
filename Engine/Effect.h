#pragma once
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

namespace Snowblind
{

	struct SVertexShader;
	struct SPixelShader;
	struct SGeometryShader;
	struct SHullShader;
	struct SDomainShader;
	struct SComputeShader;
	
	class CTexture;
	class CEffect
	{
		friend class CShaderFactory;
	public:
		CEffect(const std::string& aFilePath);
		~CEffect();

		SVertexShader* GetVertexShader();
		SPixelShader* GetPixelShader();

		/* TODO : IMPLEMENT THE FOLLOWING SHADERS */
		SGeometryShader* GetGeometryShader();
		SHullShader* GetHullShader();
		SDomainShader* GetDomainShader();
		SComputeShader* GetComputeShader();

		void ActivateShaderResources();
		void DeactivateShaderResources();
		void AddShaderResource(ID3D11ShaderResourceView* aShaderResource);
	private:
		std::string myFileName;
		ID3D11DeviceContext* myContext;

		SVertexShader* myVertexShader = nullptr;
		SPixelShader* myPixelShader = nullptr;
		SGeometryShader* myGeometryShader = nullptr;
		SHullShader* myHullShader = nullptr;
		SDomainShader* myDomainShader = nullptr;
		SComputeShader* myComputeShader = nullptr;
		
		CU::GrowingArray<ID3D11ShaderResourceView*> myShaderResources;
		CU::GrowingArray<ID3D11ShaderResourceView*> myNULLList;
		bool firstOptimize = false;
	};
};
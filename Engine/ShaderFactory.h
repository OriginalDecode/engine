#pragma once

namespace Snowblind
{
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
		void CreateShader(const std::string& aShader, SVertexShader* aVertexShader);
		void CreateShader(const std::string& aShader, SPixelShader* aPixelShader);
		void CreateShader(const std::string& aShader, SGeometryShader* aGeometryShader);
		void CreateShader(const std::string& aShader, SHullShader* aHullShader);
		void CreateShader(const std::string& aShader, SDomainShader* aDomainShader);
		void CreateShader(const std::string& aShader, SComputeShader* aComputeShader);

	};
};
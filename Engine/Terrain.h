#pragma once
#include "BaseModel.h"
#include <standard_datatype.hpp>
namespace Snowblind
{
	class CTerrain : public CBaseModel
	{
	public:
		CTerrain(u32 width, u32 height);
		CTerrain(const std::string& aFilePath);
		CTerrain();
		~CTerrain();

		void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) override;

		void Save(const std::string& aFilename);
		void Load(const std::string& aFilePath);


	private:
		void CreateVertices(u32 width, u32 height);
		void SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) override;
		CU::Matrix44f myOrientation;
		u32 myWidth;
		u32 myHeight;
	};
};
#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include "LightStructs.h"
namespace Snowblind
{
	enum class eModelType
	{
		ANIMATED,
		STATIC
	};


	class CModel;
	class CCamera;
	class CInstance
	{
	public:
		CInstance();
		~CInstance();
		void Initiate(const char* aFilePath, const std::string& effect , const eModelType& aModelType = eModelType::STATIC);
		void Initiate(CModel* aModel);
		void Update(float aDeltaTime);
		void Render(CU::Matrix44f& previousOrientation, CCamera& aCamera);
		void Render();
		void SetPosition(CU::Math::Vector3<float> aPosition);

		void UpdateLight(CU::StaticArray<SDirectionallightData, DIRECTIONAL_SIZE>& someData);
		void UpdateLight(CU::StaticArray<SSpotlightData, SPOTLIGHT_SIZE>& someData);
		void UpdateLight(CU::StaticArray<SPointlightData, POINTLIGHT_SIZE>& someData);

	private:

		void operator=(CInstance&) = delete;

		CModel* myModel;
		CU::Matrix44f myOrientation;



	};
};
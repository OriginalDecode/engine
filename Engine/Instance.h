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
		void Render(const CU::Matrix44f& previousOrientation, CCamera& aCamera);
		void SetPosition(CU::Math::Vector3<float> aPosition);
		void SetIsLightmesh();
	private:
		void operator=(CInstance&) = delete;

		CModel* myModel;
		CU::Matrix44f myOrientation;

	};
};
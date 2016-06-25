#pragma once
#include "LightStructs.h"

namespace Snowblind
{

	class CInstance;
	class CPointLight
	{
	public:
		CPointLight();
		~CPointLight();

		void Initiate(const CU::Vector3f& aPosition, const CU::Vector4f& aColor, float aRange);

		void SetPosition(const CU::Vector3f& aPosition);
		const CU::Vector3f& GetPosition();

		void SetColor(const CU::Vector4f& aColor);
		const CU::Vector4f& GetColor();
		
		void SetRange(float aRange);
		const float& GetRange();

		void Update();
		void Render(CU::Matrix44f& previousOrientation, CCamera* camera);
		const SPointlightData& GetData() const;
#ifdef _DEBUG
		CInstance* GetInstance();
#endif
		const CU::Matrix44f& GetOrientation();
	private:

#ifdef _DEBUG
		CInstance* myInstance;
#endif
		CInstance* myLightMesh;

		CU::Matrix44f myOrientation;
		CU::Vector3f myOriginalPosition;
		CU::Vector4f myColor;
		SPointlightData myData;
		float myRange;

	};
};
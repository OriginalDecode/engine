#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include "LightStructs.h"

enum class eModelType
{
	ANIMATED,
	STATIC
};


class Model;
class Camera;
class CInstance
{
public:
	CInstance() = default;
	void Initiate(const char* aFilePath, const std::string& effect, const eModelType& aModelType = eModelType::STATIC);
	void Initiate(Model* aModel);
	void Render(const CU::Matrix44f& previousOrientation, Camera& aCamera);
	void SetPosition(CU::Math::Vector3<float> aPosition);
	void SetIsLightmesh();
private:
	void operator=(CInstance&) = delete;

	Model* myModel;
	CU::Matrix44f myOrientation;

};

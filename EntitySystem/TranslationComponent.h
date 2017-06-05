#pragma once
#include "Math/Matrix/Matrix44.h"
#include "BaseComponent.h"
struct TranslationComponent : public BaseComponent
{
	CU::Matrix44f myOrientation;
} typedef TranslationComponent;
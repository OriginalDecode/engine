#pragma once
#include "Math/Matrix/Matrix44.h"
#include "BaseComponent.h"
struct STranslationComponent : public SBaseComponent
{
	CU::Matrix44f myOrientation;
} typedef TranslationComponent;
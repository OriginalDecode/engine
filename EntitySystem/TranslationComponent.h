#pragma once
#include "Math/Matrix/Matrix44.h"
#include "BaseComponent.h"
struct STranslationComponent : public SBaseComponent
{
	~STranslationComponent();
	CU::Matrix44f myOrientation;
} typedef TranslationComponent;
#pragma once
#include <string>
#include "BaseComponent.h"
struct SRenderComponent : public SBaseComponent
{
	~SRenderComponent();
	std::string myModelID;
} typedef RenderComponent;
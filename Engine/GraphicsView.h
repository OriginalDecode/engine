#pragma once
#include <Engine/InspectorView.h>
class GraphicsView : public InspectorView
{
public:
	GraphicsView(EntityManager& em);
	~GraphicsView() override;

	void Update() override;
	void EditTransform(const float *cameraView, float *cameraProjection, float* matrix);
};


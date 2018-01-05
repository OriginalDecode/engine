#pragma once
#include <Engine/InspectorView.h>
class TransformView : public InspectorView
{
public:
	TransformView(EntityManager& em);
	~TransformView() override;

	virtual void Update() override;
	void EditTransform(const float *cameraView, float *cameraProjection, float* matrix);

private:


};

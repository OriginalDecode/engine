#pragma once
#include <Engine/TransformView.h>
class GraphicsView : public TransformView
{
public:
	GraphicsView(EntityManager& em);
	~GraphicsView() override;

	void Update() override;
};


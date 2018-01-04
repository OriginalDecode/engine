#pragma once
#include "InspectorView.h"
#include <vector>

class EntityManager;
class Inspector
{
public:
	Inspector();
	~Inspector();


	void Update(float dt);
	void SetEntity(Entity e);
private:
	Entity m_CurrentEntity;
	std::vector<InspectorView*> m_Views;
	EntityManager& m_Manager;


};


#pragma once
#include <vector>

class EntityManager;
class InspectorView;
struct DebugComponent;

class Inspector
{
public:
	Inspector();
	~Inspector();


	void Update(float dt);
	void SetEntity(Entity e);
private:
	void HandleAdd();
	Entity m_CurrentEntity;
	std::vector<InspectorView*> m_Views;
	EntityManager& m_Manager;


};

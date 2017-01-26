#pragma once

#include <bitset>
#include "../EntitySystem/EntityManager.h"

class EditObject
{
public:
	EditObject() = default;

	void Initiate(Entity entity_id, s32 component_flags);
	void Update();
	enum ContainsComponent
	{
		LIGHT = 1,
		GRAPHICS = 2,
		PHYSICS = 4,
		AI = 8,
		INPUT = 16,
		NETWORK = 32,
	};
	void SetComponentToEdit(s32 component_flag);
private:
	void EditGraphics();
	void EditLight();
	void EditAI();
	void EditNetwork();
	void EditInput();
	void EditPhysics();

	Entity m_ID;
	s32 m_Components = 0;
	s32 m_EditComponent = 0;
	float m_ButtonX = 150.f;
	float m_ButtonY = 20.f;
	EntityManager* m_EntityManager;
};


#include "InputSystem.h"

#include "InputComponent.h"


InputSystem::InputSystem(CEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<InputController>>())
{
}

void InputSystem::Update(float delta_time)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (const Entity& e : entities)
	{
		
	}
}

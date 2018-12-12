#pragma once
#include "../Engine/engine_shared.h"
#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../CommonLib/DataStructures/StaticArray.h"

#define MAX_COMPONENTS_COUNT 32

struct BaseComponent;
typedef uint32 Entity;

typedef CU::GrowingArray<BaseComponent*> ComponentArray;
typedef CU::StaticArray<int, MAX_COMPONENTS_COUNT> EntityComponentArray;
//constexpr int a = sizeof(int);
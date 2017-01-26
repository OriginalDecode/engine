#pragma once
#include "../Engine/snowblind_shared.h"
#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../CommonLib/DataStructures/StaticArray.h"

#define MAX_COMPONENTS_COUNT 32

struct BaseComponent;
typedef int Entity;

typedef CU::GrowingArray<BaseComponent*> ComponentArray;
typedef CU::StaticArray<int, MAX_COMPONENTS_COUNT> EntityComponentArray;

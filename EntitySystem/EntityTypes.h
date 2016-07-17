#pragma once
#include <bitset>
#include "DataStructures/GrowingArray.h"
#include "DataStructures/StaticArray.h"

#define MAX_COMPONENTS_COUNT 32

struct SBaseComponent;
typedef unsigned int Entity;

typedef CU::GrowingArray<SBaseComponent*> ComponentArray;
typedef CU::StaticArray<int, MAX_COMPONENTS_COUNT> EntityComponentArray;

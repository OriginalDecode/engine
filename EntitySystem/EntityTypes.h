#pragma once
#include <bitset>
#include "standard_datatype.hpp"
#include "DataStructures/GrowingArray.h"
#include "DataStructures/StaticArray.h"

#define MAX_COMPONENTS_COUNT 32

struct BaseComponent;
typedef unsigned int Entity;

typedef CU::GrowingArray<BaseComponent*> ComponentArray;
typedef CU::StaticArray<int, MAX_COMPONENTS_COUNT> EntityComponentArray;

#pragma once
#include "standard_datatypes.h"

#define REGISTER_ENUM(net) net,
enum eNetMessageType : u64
{
#include "RegisterNetEnums.h"
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(str) #str,
static const char* NetMessageTypeString[] = 
{
#include "RegisterNetEnums.h"
};
#undef REGISTER_ENUM
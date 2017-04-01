#pragma once
#include "snowblind_shared.h"
#include <Math/Vector/Vector.h>

struct EntityBaseFormat
{
	s8 m_EntityBase[512] = { '\0' };
	CU::Vector3f m_Position;
}; //536byte

struct EntityFormatHeader
{
	s8 format[3] = { 's','e','f' };
	EntityBaseFormat base_format;
	s32 size;
	s8* data = nullptr;

};



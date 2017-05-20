#define Snowblind_VERSION_MAJOR 
#define Snowblind_VERSION_MINOR
#ifndef STANDARD_DATATYPES_H
#define STANDARD_DATATYPES_H

#if defined(_MSC_VER)
typedef unsigned __int8 u8;
typedef __int8 s8;
#else
typedef unsigned char u8;
typedef unsigned char s8;
#endif
typedef char c8;

#if defined(_MSC_VER)
typedef unsigned __int16 u16;
typedef __int16 s16;
#else
typedef unsigned short u16;
typedef short s16;
#endif

#if defined(_MSC_VER)
typedef unsigned __int32 u32;
typedef __int32 s32;
#else
typedef unsigned int u32;
typedef signed int s32;
#endif

#if defined(_MSC_VER)
typedef unsigned __int64 u64;
typedef __int64 s64;
#else
typedef unsigned long long u64;
typedef long long s64;
#endif
#endif
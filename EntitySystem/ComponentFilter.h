#pragma once
#include "EntityTypes.h"
#include "TypeID.h"

template<typename... Args>
struct STypeList { };

template<typename... Args>
struct Requires : STypeList<Args...> { };

template<typename... Args>
struct Excludes : STypeList<Args...> { };

template<typename... Args>
static void Types(EntityComponentArray&, STypeList<Args...>) { };

template<typename T, typename... Args>
static void Types(EntityComponentArray& someComponents, STypeList<T, Args...>)
{
	someComponents[CTypeID<SBaseComponent>::GetID<T>()] = 1;
	Types(someComponents, STypeList<Args...>());
};

struct SComponentFilter
{
public:
	SComponentFilter()
	{
		for (int i = 0; i < MAX_COMPONENTS_COUNT; i++)
		{
			myRequires[i] = 0;
			myExcludes[i] = 0;
		}
	}

	SComponentFilter(const EntityComponentArray& requiredComponents, const EntityComponentArray& excludedComponents)
		: myRequires(requiredComponents)
		, myExcludes(excludedComponents)
	{
	}

	bool Compare(const EntityComponentArray& someComponents)
	{
		for (int i = 0; i < MAX_COMPONENTS_COUNT; i++)
		{
			if (myRequires[i] == 1 && someComponents[i] == -1)
				return false;

			if (myExcludes[i] == 1 && someComponents[i] != -1)
				return false;
		}

		return true;
	}

private:

	EntityComponentArray myRequires;
	EntityComponentArray myExcludes;

};

template<typename RequireList, typename ExcludeList = Excludes<>>
SComponentFilter CreateFilter()
{
	EntityComponentArray requires;
	EntityComponentArray excludes;

	for (int i = 0; i < MAX_COMPONENTS_COUNT; i++)
	{
		requires[i] = 0;
		excludes[i] = 0;
	}

	Types(requires, RequireList{});
	Types(excludes, ExcludeList{});

	return SComponentFilter(requires, excludes);
}


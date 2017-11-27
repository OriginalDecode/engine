#pragma once
#include "EntityTypes.h"
#include "TypeID.h"

template<typename... Args>
struct TypeList { };

template<typename... Args>
struct Requires : TypeList<Args...> { };

template<typename... Args>
struct Excludes : TypeList<Args...> { };

template<typename... Args>
static void Types(EntityComponentArray&, TypeList<Args...>) { };

template<typename T, typename... Args>
static void Types(EntityComponentArray& someComponents, TypeList<T, Args...>)
{
	someComponents[CTypeID<BaseComponent>::GetID<T>()] = 1;
	Types(someComponents, TypeList<Args...>());
};

struct ComponentFilter
{
public:
	ComponentFilter()
	{
		for (int i = 0; i < MAX_COMPONENTS_COUNT; i++)
		{
			myRequires[i] = 0;
			myExcludes[i] = 0;
		}
	}

	ComponentFilter(const EntityComponentArray& requiredComponents, const EntityComponentArray& excludedComponents)
		: myRequires(requiredComponents)
		, myExcludes(excludedComponents)
	{
	}

	bool operator==(const ComponentFilter& other)
	{
		return ((*this).myRequires == other.myRequires);
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
ComponentFilter CreateFilter()
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

	return ComponentFilter(requires, excludes);
}


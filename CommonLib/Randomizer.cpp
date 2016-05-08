#include "Randomizer.h"

Randomizer* Randomizer::myInstance = nullptr;

Randomizer::Randomizer()
	: e2(myRandomDevice())
{
}

Randomizer::~Randomizer()
{
}

bool Randomizer::Create()
{
	if (myInstance == nullptr)
	{
		myInstance = new Randomizer();
		return true;
	}

	return false;
}

bool Randomizer::Destroy()
{
	if (myInstance != nullptr)
	{
		delete myInstance;
		myInstance = nullptr;
		return true;
	}
	return false;
}

Randomizer* Randomizer::GetInstance()
{
	return myInstance;
}
#include "Megaton.h"
#include "DL_Debug.h"
namespace CommonUtilities
{

	Megaton* Megaton::myInstance = nullptr;
	bool Megaton::myDebugMode = nullptr;

	Megaton::Megaton()
	{
	}


	Megaton::~Megaton()
	{
	}

	bool Megaton::Create(HINSTANCE /*someHinstance*/)
	{
		if (myInstance == nullptr)
		{
			myInstance		= new Megaton();
		}
		return true;
	}

	bool Megaton::Destroy()
	{
		//...
		SAFE_DELETE(myInstance);

		return true;
	}

}
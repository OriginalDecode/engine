#include "Megaton.h"
namespace CommonUtilities
{

	Megaton* Megaton::myInstance = nullptr;
	Input::InputWrapper* Megaton::myInput = nullptr;
	TimeManager* Megaton::myTimeManager = nullptr;
	Camera2D* Megaton::my2DCamera = nullptr;
	Camera3D* Megaton::my3DCamera = nullptr;
	XMLReader* Megaton::myXMLReader = nullptr;
	bool Megaton::myDebugMode = nullptr;

	Megaton::Megaton()
	{

	}


	Megaton::~Megaton()
	{

	}

	bool Megaton::Create()
	{
		if (myInstance == nullptr)
		{
			myInstance		= new(Megaton());
			my2DCamera		= new(Camera2D());
			my3DCamera		= new(Camera3D());
			myInput			= new(Input::InputWrapper());
			myTimeManager	= new(TimeManager());
			myXMLReader		= new(XMLReader());


			DL_DEBUG("Megaton Instance Created!");
	
		}
		else
		{
			DL_DEBUG("Instance already created, asserting!");
			DL_ASSERT("Instance of Megaton already created.!");
		}

		return true;
	}

	bool Megaton::Destroy()
	{
		//...
		SAFE_DELETE(myInput);
		SAFE_DELETE(myTimeManager);
		SAFE_DELETE(my2DCamera);
		SAFE_DELETE(my3DCamera);
		SAFE_DELETE(myXMLReader);
		SAFE_DELETE(myInstance);

		return true;
	}

}
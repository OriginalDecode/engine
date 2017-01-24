#pragma once
#include "Input\Input\InputWrapper.h"
#include "TimeManager\TimeManager\TimeManager.h"
#include "../Camera/Camera2D.h"
#include "../Camera/Camera3D.h"
#include "../XML/XMLReader.h"

#define INPUT_HANDLER		CU::Megaton::GetInputWrapper()
#define TIME_HANDLER		CU::Megaton::GetTimeManager()
#define CAMERA2D			CU::Megaton::Get2DCamera()
#define CAMERA3D			CU::Megaton::Get3DCamera()
#define MEGATON				CU::Megaton::GetInstance()
#define XMLHandler			CU::Megaton::GetXMLReader()
#define GET_DEBUG			CU::Megaton::GetDebugMode()
#define SET_DEBUG(mode)		CU::Megaton::SetDebugMode(mode)

namespace CommonUtilities
{
	class Megaton
	{
	public:


		static bool			Create();
		static bool			Destroy();

		inline static		Megaton* GetInstance();
		inline static		CU::Input::InputWrapper* GetInputWrapper();
		inline static		CU::TimeManager* GetTimeManager();
		inline static		Camera2D* Get2DCamera();
		inline static		Camera3D* Get3DCamera();
		inline static		XMLReader* GetXMLReader();
		inline static		bool GetDebugMode();
		inline static		void SetDebugMode(bool aMode);
	private:
		Megaton();
		~Megaton();

		//Classes you want to be Megaton
		static				Megaton* myInstance;
		static				CU::Input::InputWrapper* myInput;
		static				CU::TimeManager* myTimeManager;
		static				Camera2D* my2DCamera;
		static				Camera3D* my3DCamera;
		static				XMLReader* myXMLReader;




		static bool			myDebugMode;


	};


	Megaton* Megaton::GetInstance()
	{
		return myInstance;
	}

	CU::Input::InputWrapper* Megaton::GetInputWrapper()
	{
		return myInput;
	}

	CU::TimeManager* Megaton::GetTimeManager()
	{
		return myTimeManager;
	}

	Camera2D* Megaton::Get2DCamera()
	{
		return my2DCamera;
	}

	Camera3D* Megaton::Get3DCamera()
	{
		return my3DCamera;
	}

	XMLReader* Megaton::GetXMLReader()
	{
		return myXMLReader;
	}

	bool Megaton::GetDebugMode()
	{
		return myDebugMode;
	}

	void Megaton::SetDebugMode(bool aMode)
	{
		myDebugMode = aMode;
	}


}
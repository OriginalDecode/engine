#pragma once
//Add all the good stuff again in here.
#include "Global.h"

#define MEGATON				CU::Megaton::GetInstance()

#define GET_DEBUG			CU::Megaton::GetDebugMode()
#define SET_DEBUG(mode)		CU::Megaton::SetDebugMode(mode)

namespace CommonUtilities
{
	class Megaton
	{
	public:


		static bool			Create(HINSTANCE someHinstance);
		static bool			Destroy();

		inline static		Megaton* GetInstance();
		inline static		bool GetDebugMode();
		inline static		void SetDebugMode(bool aMode);
	private:
		Megaton();
		~Megaton();

		//Classes you want to be Megaton
		static				Megaton* myInstance;
		
		static bool			myDebugMode;
	};


	Megaton* Megaton::GetInstance()
	{
		return myInstance;
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
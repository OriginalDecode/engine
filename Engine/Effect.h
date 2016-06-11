#pragma once
#include <string>
#include "LightStructs.h"
#include <DataStructures/StaticArray.h>




namespace Snowblind
{
	class CTexture;
	class CEffect
	{
	public:
		CEffect();
		~CEffect();

		void Initiate(const std::wstring& aFile);
	
	private:
		std::wstring myTechniqueName;




	};
};
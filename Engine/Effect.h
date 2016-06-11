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

		void Initiate(const std::string& aFile);
	
	private:
		std::string myTechniqueName;




	};
};
#pragma once
#include "API.h"
namespace Snowblind
{
	class CDirectX12 : 
		public CAPI
	{
	public:
		CDirectX12();
		~CDirectX12();
		void Present() override;
		void Clear() override;
	};
};
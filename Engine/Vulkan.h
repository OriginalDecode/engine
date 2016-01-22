#pragma once
#include "API.h"
namespace Snowblind
{
	class CVulkan :
		public CAPI
	{
	public:
		CVulkan();
		~CVulkan();
		void Present() override;
		void Clear() override;
	};
};
#pragma once
#include "API.h"
namespace Snowblind
{
	class COpenGL :
		public CAPI
	{
	public:
		COpenGL();
		~COpenGL();
		void Present() override;
		void Clear() override;
	};
};
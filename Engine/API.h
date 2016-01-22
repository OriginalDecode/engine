#pragma once
#include <bitset>

#define IS_FULLSCREEN 1
#define IS_BORDERLESS 1
#define NOT_FULLSCREEN 0
#define NOT_BORDERLESS 0

namespace Snowblind
{
	class CAPI
	{
	public:
		CAPI();
		virtual ~CAPI();

		const char* GetAPIName() const;

		virtual void Present() = 0;
		virtual void Clear() = 0;
	protected:
		const char* myAPI;
		 
		enum eWindowFlags
		{
			FULLSCREEN,
			BORDERLESS,
			_COUNT
		};

		std::bitset<static_cast<int>(eWindowFlags::_COUNT)> myWindowFlags;

	};
};


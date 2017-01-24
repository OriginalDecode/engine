#pragma once

#include <functional>

namespace CommonUtilities
{
	class Work
	{
	public:
		Work();
		Work(std::function<void()> aFunction);
		~Work();

		void DoWork();
	private:
		std::function<void()> myWork;
	};
}
namespace CU = CommonUtilities;
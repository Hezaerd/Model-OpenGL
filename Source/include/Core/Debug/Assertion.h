#pragma once

#ifdef _DEBUG
	#define ASSERT(expr) \
		if(!expr) \
		{ \
			__debugbreak(): \
		}
#else 
	#define Assert(expr) \
		if(!expr) \
		{
			std::abort(); \
		}
#endif
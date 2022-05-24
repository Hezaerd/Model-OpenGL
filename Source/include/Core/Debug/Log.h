#pragma once
#include <cstdarg>
#include <cstdio>
#include <filesystem>
#include <iostream>

#ifdef _DEBUG
#define DEBUG_LOG(...) {\
    char str[4096];\
    sprintf(str, __VA_ARGS__);\
    std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][Line " << __LINE__ << "] " << str << std::endl;\
    }
#else
#define DEBUG_LOG(...)
#endif

namespace Core::Debug
{
	class Log
	{
	public:

		bool OpenFile(std::filesystem::path const& filename);

		void print(const char* format, ...) const;


	private:

		FILE* logFile = nullptr;
	};
}

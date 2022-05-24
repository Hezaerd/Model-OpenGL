#include "../../Source/include/pch.h"
#include "../../Source/include/Core/Debug/Log.h"

void Core::Debug::Log::print(const char* format, ...) const
{
	va_list args;
	va_start(args, format);

	if(logFile != nullptr)
	{
		va_list copy;
		va_copy(copy, args);
		vfprintf(logFile, format ,args);
		va_end(args);
	}

	vfprintf(stdout, format, args);
	va_end(args);

	fflush(logFile);
}

bool Core::Debug::Log::OpenFile(std::filesystem::path const& filename)
{
	logFile = fopen((char const*) filename.string().c_str(), "w");

	return (logFile != nullptr);
}

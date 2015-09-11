#pragma once

#include "CSpiceCore.h"

#include <sstream>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>

#define CSPICE_ASSERT(expression)																																					\
	if(failed_c())																																									\
	{																																												\
		std::stringstream errorStr;																																					\
		errorStr << "CSpice error in " << __FILE__ << " (line " << __LINE__ << "): Error flag was set prior to function call: " << CSpiceUtil::GetShortErrorMessage();				\
		CSpiceUtil::LogError(errorStr.str());																																		\
		throw std::runtime_error(errorStr.str());																																	\
	}																																												\
	expression;																																										\
	if(failed_c())																																									\
	{																																												\
		std::stringstream errorStr;																																					\
		errorStr << "CSpice error in " << __FILE__ << " (line " << __LINE__ << "): " << CSpiceUtil::GetShortErrorMessage();															\
		CSpiceUtil::LogError(errorStr.str());																																		\
		throw std::runtime_error(errorStr.str());																																	\
	}

class CSpiceUtil
{
public:
	static void SetErrorHandlingParams(const std::string& action, const std::string& device);
	static void SetLoggingFile(const std::string& file);

	static std::string GetShortErrorMessage();
	static std::string GetExplainErrorMessage();
	static std::string GetLongErrorMessage();
	static std::string GetTraceback();

	static void LogError(const std::string& extraMsg);

	static void ResetErrorFlag();

private:
	static std::string logFile;
};

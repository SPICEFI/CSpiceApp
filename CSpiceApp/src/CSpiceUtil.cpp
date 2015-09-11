#include "CSpiceUtil.h"

void CSpiceUtil::SetErrorHandlingParams(const std::string& action, const std::string& device)
{
	size_t actionLen = action.length() + 1;
	size_t deviceLen = device.length() + 1;

	char* actionCStr = new char[actionLen];
	char* deviceCStr = new char[deviceLen];

	std::strncpy(actionCStr, action.c_str(), actionLen);
	std::strncpy(deviceCStr, device.c_str(), deviceLen);

	erract_c("set", 0, actionCStr);
	errdev_c("set", 0, deviceCStr);

	delete[] actionCStr;
	delete[] deviceCStr;
}

void CSpiceUtil::SetLoggingFile(const std::string& file)
{
	logFile = file;
}

std::string CSpiceUtil::GetShortErrorMessage()
{
	char msg[SPICE_ERROR_SMSGLN];
	getmsg_c("short", SPICE_ERROR_SMSGLN, msg);

	return std::string(msg);
}

std::string CSpiceUtil::GetExplainErrorMessage()
{
	char msg[SPICE_ERROR_XMSGLN];
	getmsg_c("explain", SPICE_ERROR_XMSGLN, msg);

	return std::string(msg);
}

std::string CSpiceUtil::GetLongErrorMessage()
{
	char msg[SPICE_ERROR_LMSGLN];
	getmsg_c("long", SPICE_ERROR_LMSGLN, msg);

	return std::string(msg);
}

std::string CSpiceUtil::GetTraceback()
{
	char traceback[SPICE_ERROR_TRCLEN];
	qcktrc_c(SPICE_ERROR_TRCLEN, traceback);

	return std::string(traceback);
}

void CSpiceUtil::SignalError(const std::string& errorMsg)
{
	LogError(errorMsg);

	throw std::runtime_error(errorMsg);
}

void CSpiceUtil::LogError(const std::string& extraMsg)
{
	if(logFile == "")
		return;

	time_t now = time(nullptr);

	struct tm timeInfo;
	localtime_s(&timeInfo, &now);

	std::ofstream out(logFile, std::ofstream::app);

	out << std::put_time(&timeInfo, "%a %b %d %Y %H:%M:%S") << "\n";
	out << "\tToolkit version: " << tkvrsn_c("toolkit") << "\n";

	if(failed_c())
	{
		out << "\tShort: " << GetShortErrorMessage() << "\n";
		out << "\tExplain: " << GetExplainErrorMessage() << "\n";
		out << "\tLong: " << GetLongErrorMessage() << "\n";
		out << "\tTraceback: " << GetTraceback() << "\n";
	}

	out << "\tExtra info: " << extraMsg << "\n";
	out << "\n";

	out.close();
}

void CSpiceUtil::ResetErrorFlag()
{
	reset_c();
}

std::string CSpiceUtil::logFile = "";

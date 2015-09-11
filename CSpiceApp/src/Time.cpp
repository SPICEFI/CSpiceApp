#include "Time.h"

const double Time::second = 1.0;
const double Time::minute = 60.0 * Time::second;
const double Time::hour = 60.0 * Time::minute;
const double Time::day = 24.0 * Time::hour;

Time::Time(double et)
{
	etTime = et;
}

Time::Time(std::string str)
{
	CSPICE_ASSERT(str2et_c(str.c_str(), &etTime));
}

double Time::AsDouble() const
{
	return etTime;
}

std::string Time::AsString(const std::string& format) const
{
	char timeStr[FORMAT_STRING_BUFFER];
	CSPICE_ASSERT(timout_c(etTime, format.c_str(), FORMAT_STRING_BUFFER, timeStr));

	return std::string(timeStr);
}

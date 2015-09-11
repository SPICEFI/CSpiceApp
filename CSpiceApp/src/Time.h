#pragma once

#include <string>

#include "CSpiceCore.h"

#define FORMAT_STRING_BUFFER 1024

class Time
{
public:
	Time(double et = 0.0);
	Time(std::string str);

	double AsDouble() const;
	std::string AsString(const std::string& format = "Mon DD YYYY HR:MN:SC (UTC+0) ::UTC+0") const;

	Time operator+(double rhs)
	{
		return Time(etTime + rhs);
	}
	Time operator-(double rhs)
	{
		return (*this) + (-rhs);
	}
	double operator-(const Time& rhs)
	{
		return etTime - rhs.AsDouble();
	}
	Time& operator+=(double rhs)
	{
		etTime += rhs;
		return *this;
	}
	Time& operator-=(double rhs)
	{
		etTime -= rhs;
		return *this;
	}
	bool operator<(const Time& rhs)
	{
		return this->AsDouble() < rhs.AsDouble();
	}
	bool operator>(const Time& rhs)
	{
		return this->AsDouble() > rhs.AsDouble();
	}

public:
	const static double second;
	const static double minute;
	const static double hour;
	const static double day;

private:
	double etTime;
};

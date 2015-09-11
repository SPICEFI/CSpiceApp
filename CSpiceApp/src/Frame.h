#pragma once

#include "CSpiceCore.h"
#include "Vector3.h"
#include "Time.h"
#include "Orientation.h"

#define FRAME_NAME_MAX_LENGTH 64

class Frame
{
public:
	Frame(int spiceId, const std::string& name = "");
	Frame(const std::string& spiceName, const std::string& name = "");
	void Construct(int spiceId, const std::string& name);
	~Frame();

	long GetSpiceId() const;
	const std::string GetSpiceName() const;

	long GetCenterObjectId() const;

	Vector3 TransformVector(const Vector3& vec, const Time& t, const Frame& ref = Frame::J2000) const;
	Vector3 AxisX(const Time& t, const Frame& ref = Frame::J2000) const;
	Vector3 AxisY(const Time& t, const Frame& ref = Frame::J2000) const;
	Vector3 AxisZ(const Time& t, const Frame& ref = Frame::J2000) const;

	Orientation GetOrientation(const Time& t, const Frame& ref = Frame::J2000) const;

	static bool ValidateId(long id);

private:
	long spiceId;
	std::string name;

public:
	static const Frame J2000;
	static const Frame ECLIPJ2000;
};

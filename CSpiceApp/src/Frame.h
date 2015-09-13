#pragma once

#include "CSpiceCore.h"
#include "Vector3.h"
#include "Time.h"
#include "Orientation.h"
#include "Window.h"

#define FRAME_NAME_MAX_LENGTH 64

class Frame
{
public:
	enum FrameType
	{
		FT_INERTIAL = 1,
		FT_PCK,
		FT_CK,
		FT_TK,
		FT_DYNAMIC
	};

	struct FrameInfo
	{
		long centerId;
		FrameType frameType;
		long classId;
	};

public:
	Frame(int spiceId, const std::string& name = "");
	Frame(const std::string& spiceName, const std::string& name = "");
	~Frame();

	long GetSpiceId() const;
	std::string GetSpiceName() const;
	const std::string& GetName() const;

	//FrameType GetFrameType() const;
	//long GetCenterObjectId() const;
	FrameInfo GetFrameInfo() const;

	Vector3 TransformVector(const Vector3& vec, const Time& t, const Frame& ref = Frame::J2000) const;
	Vector3 AxisX(const Time& t, const Frame& ref = Frame::J2000) const;
	Vector3 AxisY(const Time& t, const Frame& ref = Frame::J2000) const;
	Vector3 AxisZ(const Time& t, const Frame& ref = Frame::J2000) const;

	Orientation GetOrientation(const Time& t, const Frame& ref = Frame::J2000) const;

	bool HasAvailableData() const;
	bool HasLimitedCoverage() const;
	Window GetCoverage() const;

	static bool ValidateId(long id);
	static long MakeFrameId(FrameType type, long classId);

	static std::vector<long> GetBuiltInIds();
	static std::vector<long> GetPoolIds();
	static std::vector<long> GetLoadedPckIds();

private:
	void Construct(int spiceId, const std::string& name);

private:
	long spiceId;
	std::string name;

public:
	static const Frame J2000;
	static const Frame ECLIPJ2000;
};

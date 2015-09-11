#include "Frame.h"

const Frame Frame::J2000 = Frame("J2000");
const Frame Frame::ECLIPJ2000 = Frame("ECLIPJ2000");

Frame::Frame(int spiceId, const std::string& name)
{
	Construct(spiceId, name);
}

Frame::Frame(const std::string& spiceName, const std::string& name)
{
	long spiceId;
	CSPICE_ASSERT( namfrm_c(spiceName.c_str(), &spiceId) );

	Construct(spiceId, name);
}

void Frame::Construct(int spiceId, const std::string& name)
{
	if(!ValidateId(spiceId))
		throw std::runtime_error("No such CSpice frame is defined");

	this->spiceId = spiceId;

	if(name != "")
	{
		this->name = name;
	}
	else
	{
		this->name = GetSpiceName();
	}
}

Frame::~Frame()
{
	
}

long Frame::GetSpiceId() const
{
	return spiceId;
}

const std::string Frame::GetSpiceName() const
{
	char frameName[FRAME_NAME_MAX_LENGTH];
	CSPICE_ASSERT( frmnam_c(spiceId, FRAME_NAME_MAX_LENGTH, frameName) );

	return std::string(frameName);
}

long Frame::GetCenterObjectId() const
{
	SpiceInt centerId;
	SpiceInt clssid;
	SpiceInt frclss;
	SpiceBoolean found;

	CSPICE_ASSERT(frinfo_c(spiceId, &centerId, &frclss, &clssid, &found));

	return centerId;
}

Vector3 Frame::TransformVector(const Vector3& vec, const Time& t, const Frame& ref) const
{
	double transform[3][3];

	CSPICE_ASSERT(pxform_c(GetSpiceName().c_str(), ref.GetSpiceName().c_str(), t.AsDouble(), transform));

	double axisLocal[3] = {vec.x, vec.y, vec.z};
	double axisGlobal[3];

	CSPICE_ASSERT(mxv_c(transform, axisLocal, axisGlobal));

	return Vector3(axisGlobal);
}

Vector3 Frame::AxisX(const Time& t, const Frame& ref) const
{
	return TransformVector(Vector3::i, t, ref);
}

Vector3 Frame::AxisY(const Time& t, const Frame& ref) const
{
	return TransformVector(Vector3::j, t, ref);
}

Vector3 Frame::AxisZ(const Time& t, const Frame& ref) const
{
	return TransformVector(Vector3::k, t, ref);
}

Orientation Frame::GetOrientation(const Time& t, const Frame& ref) const
{
	return Orientation(AxisX(t, ref), AxisY(t, ref), AxisZ(t, ref));
}

bool Frame::ValidateId(long id)
{
	SpiceInt centerId;
	SpiceInt clssid;
	SpiceInt frclss;
	SpiceBoolean found;

	CSPICE_ASSERT(frinfo_c(id, &centerId, &frclss, &clssid, &found));

	return found != SPICEFALSE;
}

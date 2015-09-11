#include "SpaceBody.h"

SpaceBody::SpaceBody(long spiceId, const std::string& name) : SpaceObject(spiceId, name)
{

}

SpaceBody::SpaceBody(const std::string& spiceName, const std::string& name) : SpaceObject(spiceName, name)
{

}

SpaceBody::SpaceBody(const SpaceObject& obj) : SpaceObject(obj.GetSpiceId(), obj.GetName())
{

}

SpaceBody::~SpaceBody()
{

}

SpaceBody* SpaceBody::Clone() const
{
	return new SpaceBody(*this);
}

Frame SpaceBody::GetBodyFrame() const
{
	char frameName[FRAME_NAME_MAX_LENGTH];
	long frameId;
	SpiceBoolean found;

	CSPICE_ASSERT(cidfrm_c(spiceId, FRAME_NAME_MAX_LENGTH, &frameId, frameName, &found));

	if(!found)
		throw std::runtime_error("No such frame available");

	return Frame(frameId);
}

Orientation SpaceBody::GetOrientation(const Time& t, const Frame& ref) const
{
	return GetBodyFrame().GetOrientation(t, ref);
}

bool SpaceBody::HasParameter(BulkParameter param) const
{
	switch(param)
	{
	case BP_RADIUS:
		return bodfnd_c(spiceId, "RADII") != SPICEFALSE;
	case BP_GM:
		return bodfnd_c(spiceId, "GM") != SPICEFALSE;
	case BP_MASS:
		return HasParameter(BP_GM);
	case BP_ACC:
		return HasParameter(BP_GM) && HasParameter(BP_RADIUS);
	}

	return false;
}

double SpaceBody::GetSingleDimParam(BulkParameter param) const
{
	double value = -1.0;
	long dim;

	std::vector<double> radii;
	double GM;
	double radius;

	switch(param)
	{
	case BP_RADIUS:
		radii = GetMultiDimParam(BP_RADIUS);
		value = std::pow(radii[0] * radii[1] * radii[2], 1.0 / 3.0); // volumetric mean

		break;
	case BP_GM:
		CSPICE_ASSERT( bodvcd_c(spiceId, "GM", 1, &dim, &value) );
		value *= 1000.0 * 1000.0 * 1000.0; // km^3 -> m^3

		break;
	case BP_MASS:
		GM = GetSingleDimParam(BP_GM);
		value = GM / G;

		break;
	case BP_ACC:
		GM = GetSingleDimParam(BP_GM);
		radius = GetSingleDimParam(BP_RADIUS);
		value = GM / (radius * radius);

		break;
	}

	return value;
}

std::vector<double> SpaceBody::GetMultiDimParam(BulkParameter param) const
{
	std::vector<double> values;
	long dim;

	switch(param)
	{
	case BP_RADIUS:
		double radii[3];
		CSPICE_ASSERT( bodvcd_c(spiceId, "RADII", 3, &dim, radii) );
		for(int i = 0; i < 3; i++)
			radii[i] *= 1000.0; // km -> m
		for(int i = 0; i < 3; i++)
			values.push_back(radii[i]);

		break;
	}

	return values;
}

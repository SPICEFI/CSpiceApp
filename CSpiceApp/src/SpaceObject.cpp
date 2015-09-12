#include "SpaceObject.h"

SpaceObject::SpaceObject(long spiceId, const std::string& name)
{
	Construct(spiceId, name);
}

SpaceObject::SpaceObject(const std::string& spiceName, const std::string& name)
{
	SpiceInt spiceId;
	SpiceBoolean found;
	CSPICE_ASSERT( bodn2c_c(spiceName.c_str(), &spiceId, &found) );

	if(!found)
		CSpiceUtil::SignalError("Cannot translate " + spiceName + " into ID code");

	Construct(spiceId, name);
}

void SpaceObject::Construct(long spiceId, const std::string& name)
{
	if(!ValidateId(spiceId))
		CSpiceUtil::SignalError("No such CSpice object ID code is defined: " + std::to_string(spiceId));

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

SpaceObject::~SpaceObject()
{

}

SpaceObject* SpaceObject::Clone() const
{
	return new SpaceObject(*this);
}

long SpaceObject::GetSpiceId() const
{
	return spiceId;
}

std::string SpaceObject::GetSpiceName() const
{
	char objName[OBJECT_NAME_MAX_LENGTH];
	SpiceBoolean found;
	CSPICE_ASSERT(bodc2n_c(spiceId, OBJECT_NAME_MAX_LENGTH, objName, &found));

	return std::string(objName);
}

const std::string& SpaceObject::GetName() const
{
	return name;
}

Vector3 SpaceObject::GetPosition(const Time& t, const SpaceObject& relativeTo, const Frame& frame) const
{
	double etTime = t.AsDouble();
	long observerId = relativeTo.GetSpiceId();
	std::string frameName = frame.GetSpiceName();

	double position[3];
	double lt;

	CSPICE_ASSERT( spkgps_c(this->spiceId, etTime, frameName.c_str(), observerId, position, &lt) );

	for(int i = 0; i < 3; i++)
		position[i] *= 1000.0; // km -> m

	return Vector3(position);
}

Vector3 SpaceObject::GetVelocity(const Time& t, const SpaceObject& relativeTo, const Frame& frame) const
{
	double etTime = t.AsDouble();
	long observerId = relativeTo.GetSpiceId();
	std::string frameName = frame.GetSpiceName();

	double state[6];
	double lt;

	CSPICE_ASSERT(spkgeo_c(this->spiceId, etTime, frameName.c_str(), observerId, state, &lt));

	for(int i = 0; i < 6; i++)
		state[i] *= 1000.0; // km -> m

	return Vector3(&state[3]);
}

Window SpaceObject::GetSpkCoverage() const
{
	std::vector<KernelData> kernels = CSpiceUtil::GetLoadedKernels("SPK");

	Window coverage;

	for(size_t i = 0; i < kernels.size(); i++)
	{
		std::string file = kernels[i].filename;

		CSPICE_ASSERT( spkcov_c(file.c_str(), this->spiceId, &coverage.GetSpiceCell()) );
	}

	return coverage;
}

bool SpaceObject::ValidateId(long id)
{
	char objName[OBJECT_NAME_MAX_LENGTH];
	SpiceBoolean found;
	CSPICE_ASSERT(bodc2n_c(id, OBJECT_NAME_MAX_LENGTH, objName, &found));

	return found != SPICEFALSE;
}

bool SpaceObject::IsBarycenter(long id)
{
	if(ValidateId(id) == false)
		return false;

	return IsPlanetaryBarycenter(id) || (id == SSB_SPICE_ID);
}

bool SpaceObject::IsPlanetaryBarycenter(long id)
{
	if(ValidateId(id) == false)
		return false;

	return (id > 0 && id < 10);
}

bool SpaceObject::IsPlanet(long id)
{
	if(ValidateId(id) == false)
		return false;

	return (id > 100 && id < 1000 && (id % 100) == 99);
}

bool SpaceObject::IsMoon(long id)
{
	if(ValidateId(id) == false)
		return false;

	return IsBody(id) && !IsPlanet(id);
}

bool SpaceObject::IsBody(long id)
{
	if(ValidateId(id) == false)
		return false;

	return (id > 100 && id < 1000);
}

std::vector<long> SpaceObject::FindChildObjectIds(long id)
{
	std::vector<long> ids;

	if(id == SSB_SPICE_ID)
	{
		for(int i = 1; i <= 10; i++)
		{
			if(ValidateId(i))
				ids.push_back(i);
		}
	}
	else if(IsPlanetaryBarycenter(id))
	{
		for(int i = 1; i <= 99; i++)
		{
			long satelliteId = 100 * id + i;
			if(ValidateId(satelliteId))
				ids.push_back(satelliteId);
		}
	}

	return ids;
}

long SpaceObject::FindParentObjectId(long id)
{
	if(id == SUN_SPICE_ID || IsPlanetaryBarycenter(id))
	{
		return SSB.GetSpiceId();
	}
	else if(IsBody(id))
	{
		return id / 100;
	}
	else
	{
		return id;
	}
}

const SpaceObject SpaceObject::SSB = SpaceObject(SSB_SPICE_ID, "Solar System Barycenter");
const SpaceObject SpaceObject::Sun = SpaceObject(SUN_SPICE_ID, "Sun");

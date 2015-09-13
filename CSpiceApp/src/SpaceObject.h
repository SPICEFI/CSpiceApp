#pragma once

#include <string>
#include <vector>

#include "CSpiceCore.h"
#include "Vector3.h"
#include "Frame.h"
#include "Time.h"
#include "Orientation.h"
#include "Window.h"

#define SSB_SPICE_ID 0
#define SUN_SPICE_ID 10

#define OBJECT_NAME_MAX_LENGTH 128

class SpaceObject
{
public:
	SpaceObject(long spiceId, const std::string& name = "");
	SpaceObject(const std::string& spiceName, const std::string& name = "");
	virtual ~SpaceObject();

	virtual SpaceObject* Clone() const;


	long GetSpiceId() const;
	std::string GetSpiceName() const;
	const std::string& GetName() const;

	Vector3 GetPosition(const Time& t, const SpaceObject& relativeTo = SpaceObject::SSB, const Frame& frame = Frame::J2000) const;
	Vector3 GetVelocity(const Time& t, const SpaceObject& relativeTo = SpaceObject::SSB, const Frame& frame = Frame::J2000) const;

	Window GetCoverage() const;

	static bool ValidateId(long id);

	static bool IsBarycenter(long id);
	static bool IsPlanetaryBarycenter(long id);
	static bool IsPlanet(long id);
	static bool IsMoon(long id);
	static bool IsBody(long id);

	static std::vector<long> FindChildObjectIds(long id);
	static long FindParentObjectId(long id);

private:
	void Construct(long spiceId, const std::string& name);

protected:
	long spiceId;
	std::string name;

public:
	static const SpaceObject SSB; // Solar System Barycenter
	static const SpaceObject Sun;
};

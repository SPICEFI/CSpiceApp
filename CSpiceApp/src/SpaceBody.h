#pragma once

#include "SpaceObject.h"

#include <vector>

#define G 6.67384E-11

class SpaceBody : public SpaceObject
{
public:
	enum BulkParameter
	{
		BP_RADIUS,
		BP_GM,
		BP_MASS,
		BP_ACC
	};

public:
	SpaceBody(long spiceId, const std::string& name = "");
	SpaceBody(const std::string& spiceName, const std::string& name = "");
	SpaceBody(const SpaceObject& obj);
	~SpaceBody();

	virtual SpaceBody* Clone() const;

	Frame GetBodyFrame() const;
	Orientation GetOrientation(const Time& t, const Frame& ref = Frame::J2000) const;

	bool HasParameter(BulkParameter param) const;

	double GetSingleDimParam(BulkParameter param) const;
	std::vector<double> GetMultiDimParam(BulkParameter param) const;
};

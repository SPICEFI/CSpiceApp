#pragma once

template<int length, int time, int mass>
class Unit
{
	typedef Unit<length, time, mass> ThisUnit;

public:
	Unit() // constructs base unit
	{
		Construct(1.0, 0.0);
	}

	Unit(double multiplier, double offset)
	{
		Construct(multiplier, offset);
	}

	Unit(double multiplier, double offset, const ThisUnit& ref)
	{
		double rmultiplier = ref.GetMultiplier();
		double roffset = ref.GetOffset();

		Construct(multiplier * rmultiplier, rmultiplier * offset + roffset);
	}

	double GetMultiplier() const
	{
		return multiplier;
	}

	double GetOffset() const
	{
		return offset;
	}

	static double Convert(double value, const ThisUnit& from, const ThisUnit& to)
	{
		double fmultiplier = from.GetMultiplier();
		double foffset = from.GetOffset();
		double tmultiplier = to.GetMultiplier();
		double toffset = to.GetOffset();

		double valInBase = fmultiplier * value + foffset;

		return (valInBase - toffset) / tmultiplier;
	}

private:
	void Construct(double multiplier, double offset)
	{
		this->multiplier = multiplier;
		this->offset = offset;
	}

private:
	// Unit us represented as a linear relation to the base unit
	// Say, BASE is a quantity in base unit (say, meter) and DERIVED is a quantity in some other unit (say, kilometer), then BASE = multiplier * DERIVED + offset
	double multiplier;
	double offset;
};

template<int length, int time, int mass>
class Quantity
{
	typedef Unit<length, time, mass> CurrentUnit;

public:
	Quantity(double value)
	{
		Construct(value, GetBasicUnit());
	}
	Quantity(double value, const CurrentUnit& unit)
	{
		Construct(value, unit);
	}

	double ValueIn(const CurrentUnit& ref) const
	{
		return Unit<length, time, mass>::Convert(value, unit, ref);
	}
	double ValueInBase() const
	{
		return ValueIn(GetBaseUnit());
	}

	static const CurrentUnit& GetBaseUnit()
	{
		return CurrentUnit();
	}

private:
	void Construct(double value, const CurrentUnit& unit)
	{
		this->value = value;
		this->unit = unit;
	}

private:
	double value;
	CurrentUnit unit;

public:
};

typedef Quantity<1, 0, 0> Length;
typedef Unit<1, 0, 0> LengthUnit;

typedef Quantity<0, 1, 0> Time;
typedef Unit<0, 1, 0> TimeUnit;

typedef Quantity<1, -1, 0> Velocity;
typedef Unit<1, -1, 0> VelocityUnit;

typedef Quantity<0, 0, 1> Mass;
typedef Unit<0, 0, 1> MassUnit;

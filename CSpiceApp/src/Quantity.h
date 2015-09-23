#pragma once

#include <cmath>

double powi(double val, int exp);

template<int length, int time, int mass>
class Unit
{
	typedef Unit<length, time, mass> ThisUnit;

	typedef Unit<1, 0, 0> LengthUnit;
	typedef Unit<0, 0, 1> MassUnit;
	typedef Unit<0, 1, 0> TimeUnit;

public:
	static ThisUnit BaseUnit()
	{
		return ThisUnit();
	}
	static ThisUnit ScaledUnit(double multiplier, const ThisUnit& ref = ThisUnit())
	{
		return ThisUnit(multiplier, ref);
	}
	static ThisUnit DerivedUnit(const LengthUnit& lengthUnit, const TimeUnit& timeUnit = TimeUnit::BaseUnit(), const MassUnit& massUnit = MassUnit::BaseUnit())
	{
		double totalMultiplier = 1.0;

		totalMultiplier *= powi(lengthUnit.GetMultiplier(), length);
		totalMultiplier *= powi(timeUnit.GetMultiplier(), time);
		totalMultiplier *= powi(massUnit.GetMultiplier(), mass);

		return ThisUnit(totalMultiplier);
	}
	static ThisUnit DerivedUnit(const TimeUnit& timeUnit, const Unit<0, 0, 1>& massUnit = MassUnit::BaseUnit())
	{
		return DerivedUnit(LengthUnit::BaseUnit(), timeUnit, massUnit);
	}
	static ThisUnit DerivedUnit(const MassUnit& massUnit)
	{
		return DerivedUnit(LengthUnit::BaseUnit(), TimeUnit::BaseUnit(), massUnit);
	}

	Unit() // constructs base unit
	{
		Construct(1.0);
	}

private:
	Unit(double multiplier)
	{
		Construct(multiplier);
	}

	Unit(double multiplier, const ThisUnit& ref)
	{
		double rmultiplier = ref.GetMultiplier();

		Construct(multiplier * rmultiplier);
	}

public:
	double GetMultiplier() const
	{
		return multiplier;
	}

	static double Convert(double value, const ThisUnit& from, const ThisUnit& to)
	{
		double fmultiplier = from.GetMultiplier();
		double tmultiplier = to.GetMultiplier();

		return value * (fmultiplier / tmultiplier);
	}

private:
	void Construct(double multiplier)
	{
		this->multiplier = multiplier;
	}

private:
	double multiplier;
};

template<int length, int time, int mass>
class Quantity
{
	typedef Unit<length, time, mass> CurrentUnit;
	typedef Quantity<length, time, mass> ThisQuantity;

public:
	Quantity()
	{
		Construct(0.0, CurrentUnit::BaseUnit());
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
		return ValueIn(CurrentUnit::BaseUnit());
	}

	ThisQuantity& operator+=(const ThisQuantity& rhs)
	{
		value += rhs.ValueIn(unit);

		return *this;
	}
	ThisQuantity& operator-=(const ThisQuantity& rhs)
	{
		value -= rhs.ValueIn(unit);

		return *this;
	}
	ThisQuantity& operator*=(double rhs)
	{
		value *= rhs;

		return *this;
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

	friend Quantity operator+(const Quantity& lhs, const Quantity& rhs)
	{
		double newValue = lhs.value + rhs.ValueIn(lhs.unit);

		return Quantity(newValue, lhs.unit);
	}
	friend Quantity operator-(const Quantity& op)
	{
		return (-1.0) * op;
	}
	friend Quantity operator-(const Quantity& lhs, const Quantity& rhs)
	{
		return lhs + (-rhs);
	}
	friend Quantity operator*(const Quantity& lhs, double rhs)
	{
		return Quantity(lhs.value * rhs, lhs.unit);
	}
	friend Quantity operator*(double lhs, const Quantity& rhs)
	{
		return rhs * lhs;
	}
	friend double operator/(const Quantity& lhs, const Quantity& rhs)
	{
		return lhs.value / rhs.ValueIn(lhs.unit);
	}
	friend bool operator>(const Quantity& lhs, const Quantity& rhs)
	{
		return lhs.ValueInBase() > lhs.ValueInBase();
	}
	friend bool operator<(const Quantity& lhs, const Quantity& rhs)
	{
		return lhs.ValueInBase() < lhs.ValueInBase();
	}
};


typedef Quantity<1, 0, 0> Length;
typedef Quantity<0, 1, 0> Time;
typedef Quantity<0, 0, 1> Mass;
typedef Quantity<1, -1, 0> Velocity;
typedef Quantity<1, -2, 0> Acceleration;
typedef Quantity<3, -2, 0> GravitationalParameter;

typedef Unit<1, 0, 0> LengthUnit;
typedef Unit<0, 1, 0> TimeUnit;
typedef Unit<0, 0, 1> MassUnit;
typedef Unit<1, -1, 0> VelocityUnit;
typedef Unit<1, -2, 0> AccelerationUnit;
typedef Unit<3, -2, 0> GravitationalParameterUnit;

namespace Units
{
	class Metric
	{
	public:
		static const LengthUnit centimeters;
		static const LengthUnit meters;
		static const LengthUnit kilometers;

		static const MassUnit grams;
		static const MassUnit kilograms;
		static const MassUnit tons;

		static const VelocityUnit kmph;
		static const VelocityUnit mps;

		static const AccelerationUnit ms2;

		static const GravitationalParameterUnit gm;
	};

	class Imperial
	{
	public:
		static const LengthUnit inches;
		static const LengthUnit feet;
		static const LengthUnit yards;
		static const LengthUnit miles;

		static const MassUnit ounces;
		static const MassUnit pounds;
		static const MassUnit tons;

		static const VelocityUnit mph;
	};

	class Common
	{
	public:
		static const TimeUnit seconds;
		static const TimeUnit minutes;
		static const TimeUnit hours;
		static const TimeUnit days;
	};
}

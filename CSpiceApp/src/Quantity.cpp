#include "Quantity.h"

double powi(double val, int exp)
{
	if(exp == 0)
		return 1.0;

	if(exp < 0)
		return 1.0 / powi(val, -exp);

	double result = 1.0;

	for(int power = 0; power < exp; power++)
	{
		result *= val;
	}

	return result;
}

// Common
const TimeUnit Units::Common::seconds = TimeUnit::BaseUnit();
const TimeUnit Units::Common::minutes = TimeUnit::ScaledUnit(60.0, Units::Common::seconds);
const TimeUnit Units::Common::hours = TimeUnit::ScaledUnit(60.0, Units::Common::minutes);
const TimeUnit Units::Common::days = TimeUnit::ScaledUnit(24.0, Units::Common::hours);

// Metric
const LengthUnit Units::Metric::meters = LengthUnit::BaseUnit();
const LengthUnit Units::Metric::centimeters = LengthUnit::ScaledUnit(0.01, Units::Metric::meters);
const LengthUnit Units::Metric::kilometers = LengthUnit::ScaledUnit(1000.0, Units::Metric::meters);

const MassUnit Units::Metric::kilograms = MassUnit::BaseUnit();
const MassUnit Units::Metric::grams = MassUnit::ScaledUnit(0.001, Units::Metric::kilograms);
const MassUnit Units::Metric::tons = MassUnit::ScaledUnit(1000.0, Units::Metric::kilograms);

const VelocityUnit Units::Metric::kmph = VelocityUnit::DerivedUnit(Units::Metric::kilometers, Units::Common::hours);
const VelocityUnit Units::Metric::mps = VelocityUnit::DerivedUnit(Units::Metric::meters, Units::Common::seconds);

const AccelerationUnit Units::Metric::ms2 = AccelerationUnit::DerivedUnit(Units::Metric::meters, Units::Common::seconds);

// Imperial
const LengthUnit Units::Imperial::inches = LengthUnit::ScaledUnit(0.0254, Units::Metric::meters);
const LengthUnit Units::Imperial::feet = LengthUnit::ScaledUnit(0.3048, Units::Metric::meters);
const LengthUnit Units::Imperial::yards = LengthUnit::ScaledUnit(3.0, Units::Imperial::feet);
const LengthUnit Units::Imperial::miles = LengthUnit::ScaledUnit(5280.0, Units::Imperial::feet);

const MassUnit Units::Imperial::pounds = MassUnit::ScaledUnit(453.59237, Units::Metric::grams);
const MassUnit Units::Imperial::ounces = MassUnit::ScaledUnit(1.0/16.0, Units::Imperial::pounds);
const MassUnit Units::Imperial::tons = MassUnit::ScaledUnit(2240.0, Units::Imperial::pounds);

const VelocityUnit Units::Imperial::mph = VelocityUnit::DerivedUnit(Units::Imperial::miles, Units::Common::hours);

// Common
const GravitationalParameterUnit Units::Metric::gm = GravitationalParameterUnit::DerivedUnit(Units::Metric::kilometers, Units::Common::seconds);

#include "Main.h"

int main()
{
	try
	{
		App app;
		app.SetLoggingFile("error.log");

		app.LoadKernel("data/meta.tmk");

		SpaceBody earth("earth");
		SpaceBody moon(301);
		SpaceBody mars("maRS", "Mars");
		SpaceObject uranus_barycenter(7, "U.B.");
		SpaceBody sun = SpaceObject::Sun;

		//app.AddObject(earth);
		//app.AddObject(moon);
		//app.AddObject(mars);
		//app.AddObject(uranus_barycenter);
		app.AddObject(sun);
		//app.AddObject(earth); // Has no effect

		//app.LoadChildren(SpaceObject("mars barycenter"));

		//app.LoadSolarSystem(); // Will not re-add existing objects
		//app.LoadSolarSystem(true); // Load only planets

		app.AddObject(SpaceBody("METHONE"));

		size_t objectsCount = app.GetObjectsLength();
		for(size_t i = 0; i < objectsCount; i++)
		{
			const SpaceObject& obj = app.GetObject(i);

			std::cout << obj.GetName() << " summary:" << std::endl;

			try
			{
				const SpaceBody& body = dynamic_cast<const SpaceBody&>(obj);

				std::cout << "Bulk parameters:" << std::endl;

				if(body.HasParameter(SpaceBody::BP_MASS))
					std::cout << "\tMass: " << body.GetSingleDimParam(SpaceBody::BP_MASS) << " kg" << std::endl;
				if(body.HasParameter(SpaceBody::BP_GM))
					std::cout << "\tGM: " << body.GetSingleDimParam(SpaceBody::BP_GM) << " m^3 / s^2" << std::endl;
				if(body.HasParameter(SpaceBody::BP_ACC))
					std::cout << "\tg: " << body.GetSingleDimParam(SpaceBody::BP_ACC) << " m / s^2" << std::endl;
				if(body.HasParameter(SpaceBody::BP_RADIUS))
				{
					double radius = body.GetSingleDimParam(SpaceBody::BP_RADIUS);
					std::vector<double> radii = body.GetMultiDimParam(SpaceBody::BP_RADIUS);
					std::cout << "\tRadius: " << radius << " (" << radii[0] << ", " << radii[1] << ", " << radii[2] << ") m" << std::endl;
				}

				std::cout << std::endl;
			}
			catch(const std::bad_cast&)
			{

			}

			Window coverage = obj.GetSpkCoverage();

			std::vector<Interval> intervals = coverage.GetIntervals();

			std::cout << "SPK Coverage:" << std::endl;
			if(intervals.size() == 0)
			{
				std::cout << "\tObject does not contain any state data" << std::endl;
			}

			for(size_t i = 0; i < intervals.size(); i++)
			{
				Interval interval = intervals[i];

				Time begin(interval.GetLeft());
				Time end(interval.GetRight());

				std::cout << "\t" << begin.AsString() << " - " << end.AsString() << std::endl;
			}
			std::cout << std::endl;

			Time t("Aug 17 2200 15:51:01 UTC-5");
			//Time t("Aug 17 2000 15:51:01 UTC-5");

			t += 7 * Time::hour; // + 7 hours
			t -= 7 * Time::day; // - 1 week

			Time t2 = t - Time::second;

			bool time_cmp = t < t2; // false

			std::cout << "State and orientation on " << t.AsString() << ":" << std::endl;

			if(coverage.IsIncluded(t.AsDouble()))
			{
				Vector3 pos = obj.GetPosition(t, SpaceObject::SSB, Frame::J2000);
				Vector3 vel = obj.GetVelocity(t, SpaceObject::SSB, Frame::J2000);

				std::cout << "\tPos: (" << pos.x << ", " << pos.y << ", " << pos.z << ") m" << std::endl;
				std::cout << "\tVel: (" << vel.x << ", " << vel.y << ", " << vel.z << ") m/s" << std::endl;
			}
			else
			{
				std::cout << "\tNo state data on this epoch" << std::endl;
			}

			try
			{
				const SpaceBody& body = dynamic_cast<const SpaceBody&>(obj);

				Frame objFrame = body.GetBodyFrame();
				Orientation orient = objFrame.GetOrientation(t, Frame::J2000);
				//Orientation orient = body.GetOrientation(t, Frame::J2000); // also accepted

				Vector3 axisX = objFrame.AxisX(t);
				Vector3 axisY = objFrame.AxisY(t);
				Vector3 axisZ = objFrame.AxisZ(t);

				std::cout << "\tOrientation:" << std::endl;

				std::cout << "\t\tX axis: (" << axisX.x << ", " << axisX.y << ", " << axisX.z << ")" << std::endl;
				std::cout << "\t\tY axis: (" << axisY.x << ", " << axisY.y << ", " << axisY.z << ")" << std::endl;
				std::cout << "\t\tZ axis: (" << axisZ.x << ", " << axisZ.y << ", " << axisZ.z << ")" << std::endl;

				// use for glRotate, tilt then rotate
				Vector3 tiltAxis = orient.GetTiltAxis();
				double tiltAngle = orient.GetTiltAngle();
				Vector3 rotationAxis = orient.GetRotationAxis();
				double rotationAngle = orient.GetRotationAngle();
			}
			catch(const std::runtime_error&)
			{
				std::cout << "\tNo orientation data available" << std::endl;
				CSpiceUtil::ResetErrorFlag();
			}
			catch(const std::bad_cast&)
			{

			}

			std::cout << "=======================================================================" << std::endl;
			std::cout << std::endl;
		}

		std::cout << "All is fine" << std::endl;
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	std::getchar();
	return 0;
}

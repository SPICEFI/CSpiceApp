#include "Main.h"

int main()
{
	std::ofstream fout;
	fout.open("out.txt");

	try
	{
		App app;
		app.SetLoggingFile("error.log");

		app.LoadKernel("data/meta.tm");

		app.SetReferenceFrame(Frame::ECLIPJ2000); // Set reference frame to ECLIPJ200, default is J2000

		app.LoadChildren(SpaceObject("Solar system barycenter"), true, true);

		std::vector<KernelData> kernels = CSpiceUtil::GetLoadedKernels();

		fout << "Loaded kernels:" << std::endl;
		for(size_t i = 0; i < kernels.size(); i++)
		{
			KernelData kData = kernels[i];
			fout << "\t" << kData.filename << "(" << kData.type << ")" << std::endl;
		}
		fout << std::endl;

		size_t objectsCount = app.GetObjectsLength();
		for(size_t i = 0; i < objectsCount; i++)
		{
			const SpaceObject& obj = app.GetObject(i);

			fout << obj.GetName() << " summary:" << std::endl;

			try
			{
				const SpaceBody& body = dynamic_cast<const SpaceBody&>(obj);

				fout << "Bulk parameters:" << std::endl;

				if(body.HasParameter(SpaceBody::BP_MASS))
					fout << "\tMass: " << body.GetSingleDimParam(SpaceBody::BP_MASS) << " kg" << std::endl;
				if(body.HasParameter(SpaceBody::BP_GM))
					fout << "\tGM: " << body.GetSingleDimParam(SpaceBody::BP_GM) << " m^3 / s^2" << std::endl;
				if(body.HasParameter(SpaceBody::BP_ACC))
					fout << "\tg: " << body.GetSingleDimParam(SpaceBody::BP_ACC) << " m / s^2" << std::endl;
				if(body.HasParameter(SpaceBody::BP_RADIUS))
				{
					double radius = body.GetSingleDimParam(SpaceBody::BP_RADIUS);
					std::vector<double> radii = body.GetMultiDimParam(SpaceBody::BP_RADIUS);
					fout << "\tRadius: " << radius << " (" << radii[0] << ", " << radii[1] << ", " << radii[2] << ") m" << std::endl;
				}

				fout << std::endl;
			}
			catch(const std::bad_cast&)
			{

			}

			Time t("Aug 17 2000 15:51:01 UTC-5");

			Window spkCoverage = obj.GetCoverage();
			std::vector<Interval> spkIntervals = spkCoverage.GetIntervals();

			fout << "SPK state:" << std::endl;

			fout << "\tCoverage:" << std::endl;
			if(spkIntervals.size() == 0)
			{
				fout << "\t\tObject does not contain any state data" << std::endl;
			}

			for(size_t i = 0; i < spkIntervals.size(); i++)
			{
				Interval interval = spkIntervals[i];

				Time begin(interval.GetLeft());
				Time end(interval.GetRight());

				fout << "\t\t" << begin.AsString() << " - " << end.AsString() << std::endl;
			}
			fout << std::endl;

			fout << "\t" << t.AsString() << " relative to " << app.GetReferenceFrame().GetName() << ":" << std::endl;

			if(spkCoverage.IsIncluded(t.AsDouble()))
			{
				Vector3 pos = obj.GetPosition(t, SpaceObject::SSB, app.GetReferenceFrame());
				Vector3 vel = obj.GetVelocity(t, SpaceObject::SSB, app.GetReferenceFrame());

				fout << "\t\tPos: (" << pos.x << ", " << pos.y << ", " << pos.z << ") m" << std::endl;
				fout << "\t\tVel: (" << vel.x << ", " << vel.y << ", " << vel.z << ") m/s" << std::endl;

				//fout << "\t\tPos: (" << pos.x << ", " << pos.y << ", " << pos.z << ") m, abs=" << pos.Length() << " m" << std::endl;
				//fout << "\t\tVel: (" << vel.x << ", " << vel.y << ", " << vel.z << ") m/s, abs=" << vel.Length() << " m/s" << std::endl;
			}
			else
			{
				fout << "\t\tNo state data on this epoch" << std::endl;
			}
			fout << std::endl;

			try
			{
				const SpaceBody& body = dynamic_cast<const SpaceBody&>(obj);

				fout << "PCK orientation:" << std::endl;

				if(body.HasDefaultFrame())
				{					
					Frame bodyFrame = body.GetDefaultFrame();

					fout << "(using frame " << bodyFrame.GetName() << ")" << std::endl;

					if(bodyFrame.HasAvailableData())
					{
						bool dataAvailableAtT = true;

						fout << "\tCoverage:" << std::endl;
						if(bodyFrame.HasLimitedCoverage())
						{
							Window pckCoverage = bodyFrame.GetCoverage();
							std::vector<Interval> pckIntervals = pckCoverage.GetIntervals();

							dataAvailableAtT = pckCoverage.IsIncluded(t.AsDouble());

							if(pckIntervals.size() == 0)
							{
								fout << "\t\tObject does not contain any orientation data" << std::endl;
							}
							for(size_t i = 0; i < pckIntervals.size(); i++)
							{
								Interval interval = pckIntervals[i];

								Time begin(interval.GetLeft());
								Time end(interval.GetRight());

								fout << "\t\t" << begin.AsString() << " - " << end.AsString() << std::endl;
							}
							fout << std::endl;
						}
						else
						{
							fout << "\t\tUnlimited" << std::endl;
						}

						fout << "\t" << t.AsString() << app.GetReferenceFrame().GetName() << ":" << std::endl;

						if(dataAvailableAtT)
						{
							Vector3 axisX = bodyFrame.AxisX(t, app.GetReferenceFrame());
							Vector3 axisY = bodyFrame.AxisY(t, app.GetReferenceFrame());
							Vector3 axisZ = bodyFrame.AxisZ(t, app.GetReferenceFrame());

							fout << "\t\tX axis: (" << axisX.x << ", " << axisX.y << ", " << axisX.z << ")" << std::endl;
							fout << "\t\tY axis: (" << axisY.x << ", " << axisY.y << ", " << axisY.z << ")" << std::endl;
							fout << "\t\tZ axis: (" << axisZ.x << ", " << axisZ.y << ", " << axisZ.z << ")" << std::endl;
						}
						else
						{
							fout << "\t\tNo orientation data available on this epoch" << std::endl;
						}
					}
					else
					{
						fout << "\tFrame does not contain any orientation data" << std::endl;
					}
				}
				else
				{
					fout << "\tObject does not contain any orientation data" << std::endl;
				}
			}
			catch(const std::bad_cast&)
			{

			}

			fout << "=======================================================================" << std::endl;
			fout << std::endl;
		}

		std::cout << "All is fine. See out.txt for results" << std::endl;
	}
	catch(const std::exception& ex)
	{
		std::cout << std::endl;

		std::cout << "Error encountered:" << std::endl;
		std::cout << ex.what() << std::endl;
		std::cout << "See details in the error log file" << std::endl;
	}

	std::getchar();
	return 0;
}

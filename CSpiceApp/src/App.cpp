#include "App.h"

App::App() : refFrame(REF_FRAME_DEFAULT)
{
	Init();
}

App::~App()
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		delete objects[i];
		objects[i] = nullptr;
	}

	objects.clear();
}

void App::Init() const
{
	CSpiceUtil::SetErrorHandlingParams("return", "null");
}

void App::LoadKernel(const std::string& file) const
{
	CSpiceUtil::LoadKernel(file);
}

void App::SetLoggingFile(const std::string& file) const
{
	CSpiceUtil::SetLoggingFile(file);
}

void App::SetReferenceFrame(const Frame& ref)
{
	refFrame = ref;
}

const Frame& App::GetReferenceFrame() const
{
	return refFrame;
}

void App::LoadChildren(const SpaceObject& parent, bool includeSelf, bool recursive)
{
	long parentId = parent.GetSpiceId();

	std::vector<long> childrenIds = SpaceObject::FindChildObjectIds(parentId);

	if(includeSelf)
	{
		AddObject(parent);
	}

	for(size_t i = 0; i < childrenIds.size(); i++)
	{
		long childId = childrenIds[i];

		if(SpaceObject::IsBody(childId))
			AddObject(SpaceBody(childId));
		else
			AddObject(SpaceObject(childId));

		if(recursive)
			LoadChildren(SpaceObject(childId), false);
	}
}

void App::LoadSolarSystem(bool onlyPlanets)
{
	if(onlyPlanets)
	{
		std::vector<long> barycenterIds = SpaceObject::FindChildObjectIds(SpaceObject::SSB.GetSpiceId());

		for(size_t i = 0; i < barycenterIds.size(); i++)
		{
			long planetId = barycenterIds[i] * 100 + 99;

			if(SpaceObject::IsPlanet(planetId))
				AddObject(SpaceBody(planetId));
		}
	}
	else
	{
		LoadChildren(SpaceObject::SSB, true, true);
	}
}

void App::AddObject(const SpaceObject& obj)
{
	try
	{
		FindObject(obj);
	}
	catch(const std::runtime_error&)
	{
		objects.push_back(obj.Clone());
	}
}

size_t App::GetObjectsLength() const
{
	return objects.size();
}

SpaceObject& App::GetObject(size_t idx)
{
	return *objects[idx];
}

SpaceObject& App::FindObject(long id)
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->GetSpiceId() == id)
			return *objects[i];
	}

	throw std::runtime_error("FindObject haven't found requested object");
}

SpaceObject& App::FindObject(const std::string& name)
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->GetName() == name)
			return *objects[i];
	}

	try
	{
		return FindObject(SpaceObject(name));
	}
	catch(const std::runtime_error&)
	{
		throw std::runtime_error("FindObject haven't found requested object");
	}
}

SpaceObject& App::FindObject(const SpaceObject& sample)
{
	return FindObject(sample.GetSpiceId());
}

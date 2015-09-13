#pragma once

#include "CSpice.h"

#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>

#define REF_FRAME_DEFAULT Frame::J2000

class App
{
public:
	App();
	~App();

	void Init() const;
	void LoadKernel(const std::string& file) const;
	void SetLoggingFile(const std::string& file) const;

	void SetReferenceFrame(const Frame& ref);
	const Frame& GetReferenceFrame() const;

	void LoadChildren(const SpaceObject& parent, bool includeSelf = false, bool recursive = true);
	void LoadSolarSystem(bool onlyPlanets = false);

	void AddObject(const SpaceObject& obj);

	size_t GetObjectsLength() const;
	SpaceObject& GetObject(size_t idx);

	SpaceObject& FindObject(long id);
	SpaceObject& FindObject(const std::string& name);
	SpaceObject& FindObject(const SpaceObject& sample);

private:
	std::vector<SpaceObject*> objects;
	Frame refFrame;
};

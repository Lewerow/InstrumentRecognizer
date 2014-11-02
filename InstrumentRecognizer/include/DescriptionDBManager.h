#pragma once

#include "common_defs.h"

#include <ostream>
#include <mutex>
#include <map>

#include <boost/filesystem.hpp>


class DescriptionDBManager
{
public:
	virtual void startGathering() = 0;
	virtual void finishGathering() = 0;
	virtual void reset() = 0;
	
	virtual void loadDescriptions(const std::string&) = 0;
	virtual void saveDescriptions(const std::string&) = 0;

	virtual void addDescription(const ClassName&, const ObjectDescription&) = 0;

	virtual ClassDescriptionBase getTrainingDescriptions() const = 0;
	virtual ClassDescriptionBase getTestDescriptions() const = 0;
	virtual void nextCrossValidationSet() = 0;
	virtual void previousCrossValidationSet() = 0;


	virtual ~DescriptionDBManager() {}

	struct InvalidNumberOfDescriptors : std::runtime_error
	{
		InvalidNumberOfDescriptors(std::string msg) : std::runtime_error(msg) {}
	};
};

class FileDescriptionDBManager : public DescriptionDBManager
{
public:
	FileDescriptionDBManager(const boost::filesystem::path& path, int foldCount);
	virtual ~FileDescriptionDBManager(void);
	
	virtual void startGathering();
	virtual void finishGathering();
	virtual void reset();

	virtual void addDescription(const ClassName&, const ObjectDescription&);
	virtual void loadDescriptions(const boost::filesystem::path&);
	virtual void saveDescriptions(const boost::filesystem::path&);
	virtual void loadDescriptions(const std::string&);
	virtual void saveDescriptions(const std::string&);
	virtual ClassDescriptionBase getTrainingDescriptions() const;
	virtual ClassDescriptionBase getTestDescriptions() const;
	void nextCrossValidationSet();
	void previousCrossValidationSet();

	struct InvalidFile : std::runtime_error
	{
		InvalidFile(std::string msg) : std::runtime_error(msg) {}
	};

protected:
	std::vector<ClassDescriptionBase> descriptions;
	std::map<ClassName, std::size_t> currentDescriptionLocation;
	std::size_t currentOddOneOut;

	void checkIfHasValidNumberOfDescriptors(const ObjectDescription&);
	
	virtual void loadDescription(const boost::filesystem::path&);
	virtual void saveDescription(const boost::filesystem::path&, const ClassName&, const ClassDescription&);
private:
	bool gatheringEnabled;
	unsigned int descriptorsPerDecription;
	boost::filesystem::path dbPath;

	std::recursive_mutex descriptionManagerMutex;
	
	ObjectDescription loadObjectDescription(const std::string& inputStr) const;

	FileDescriptionDBManager(const FileDescriptionDBManager&) = delete;
	FileDescriptionDBManager& operator=(const FileDescriptionDBManager&) = delete;

};


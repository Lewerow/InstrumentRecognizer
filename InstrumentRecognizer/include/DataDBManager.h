#pragma once

#include "common_defs.h"

#include <string>
#include <queue>
#include <unordered_map>

#include <istream>
#include <mutex>

#include <boost/filesystem.hpp>

typedef std::string DataDBKey;

struct DataRecord
{
	DataRecord(DataDBKey key, std::istream& data, ClassName cName) : dbKey(key), input(data), className(cName) 
	{}

	DataDBKey dbKey;
	ClassName className;
	std::istream& input;

	DataRecord operator=(DataRecord&) = delete;
};

class DataDBManager
{
public:
	
	virtual bool areRecordsAvailable() = 0;
	
	virtual DataRecord take() = 0;
	virtual void release(DataDBKey key) = 0;
	
	struct InvalidPathException : public std::invalid_argument 
	{
		InvalidPathException(const std::string arg) : std::invalid_argument(arg) {}
	};

	struct StreamOpeningException : public std::runtime_error 
	{
		StreamOpeningException(const std::string arg) : std::runtime_error(arg) {}
	};

	struct NoMoreRecordsException : public std::out_of_range
	{
		NoMoreRecordsException(const std::string arg) : std::out_of_range(arg) {}
	};
};

class FileDataDBManager : public DataDBManager
{
public:
	FileDataDBManager(std::string root, std::string extension);
	
	unsigned int filesInBufferCount();
	unsigned int dirsInBufferCount();
	unsigned int currentlyProcessedCount();
	virtual bool areRecordsAvailable();

	virtual DataRecord take();
	virtual void release(DataDBKey key);
	
	virtual ~FileDataDBManager();
	
private:
	std::queue<boost::filesystem::path> remainingFiles;
	std::queue<boost::filesystem::path> remainingDirs;
	std::unordered_map<std::string, std::ifstream> currentlyProcessed;

	std::recursive_mutex dataManagerMutex;
	std::string managedExtension;

	void getFiles(boost::filesystem::path);
	void getDirs(boost::filesystem::path);
	void loadFilesFromNextDir();

	ClassName determineClassName(DataDBKey key);

	FileDataDBManager(const FileDataDBManager&) = delete;
	FileDataDBManager(const FileDataDBManager&&) = delete;
	FileDataDBManager& operator=(const FileDataDBManager&) = delete;
	FileDataDBManager& operator=(const FileDataDBManager&&) = delete;
};
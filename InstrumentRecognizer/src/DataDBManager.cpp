#include "DataDBManager.h"

#include <boost/algorithm/string.hpp>

#include <string>
#include <vector>
#include <fstream>

using namespace boost::filesystem;

FileDataDBManager::FileDataDBManager(std::string root, std::string ext) : managedExtension(ext)
{
	path p(root);
	if(exists(p))
	{
		getFiles(p);
		getDirs(p);
	}
	else
		throw InvalidPathException(root + ": path does not exist");
}

void FileDataDBManager::getFiles(path p)
{
	std::lock_guard<std::recursive_mutex> lock(dataManagerMutex);
	for(auto i = directory_iterator(p); i != directory_iterator(); i++)
	{
		if(extension(i->path()) == managedExtension) 
			remainingFiles.push(i->path());
	}
}

void FileDataDBManager::getDirs(path p)
{
	std::lock_guard<std::recursive_mutex> lock(dataManagerMutex);
	for(auto i = directory_iterator(p); i != directory_iterator(); i++)
	{
		if(is_directory(i->path())) 
			remainingDirs.push(i->path());
	}
}

DataRecord FileDataDBManager::take()
{
	std::lock_guard<std::recursive_mutex> lock(dataManagerMutex);		
	if(!areRecordsAvailable())
	    throw NoMoreRecordsException("No more records left");

	path p = remainingFiles.front();
	remainingFiles.pop();
	
	std::string key = absolute(p).string();
	IR_ASSERT(currentlyProcessed.count(key) == 0, key + " File with given key is already processed, but there is an attempt to take it");

	currentlyProcessed.insert(make_pair(key, std::ifstream(p.string())));
	if(!currentlyProcessed.at(key).is_open())
		throw StreamOpeningException("Could not open file " + p.string());

	return DataRecord(key, currentlyProcessed.at(key), determineClassName(key));
}

void FileDataDBManager::release(DataDBKey key)
{
	std::lock_guard<std::recursive_mutex> lock(dataManagerMutex);
	IR_ASSERT(currentlyProcessed.count(key) == 1, key + "File with given key is not processed, but there was an attempt to release it");
	std::ifstream& record = currentlyProcessed.at(key);

	record.close();
	currentlyProcessed.erase(key);
}

void FileDataDBManager::loadFilesFromNextDir()
{
	std::lock_guard<std::recursive_mutex> lock(dataManagerMutex);
	path p = remainingDirs.front();
	remainingDirs.pop();
	getDirs(p.string());
	getFiles(p.string());
}

unsigned int FileDataDBManager::filesInBufferCount()
{
	return remainingFiles.size();
}


unsigned int FileDataDBManager::dirsInBufferCount()
{
	return remainingDirs.size();
}


unsigned int FileDataDBManager::currentlyProcessedCount()
{
	return currentlyProcessed.size();
}

bool FileDataDBManager::areRecordsAvailable()
{
	if(!remainingFiles.empty())
		return true;
	
	std::lock_guard<std::recursive_mutex> lock(dataManagerMutex);
	while(remainingFiles.empty() && !remainingDirs.empty())
		loadFilesFromNextDir();

	return !remainingFiles.empty();
}

FileDataDBManager::~FileDataDBManager()
{
	IR_ASSERT(currentlyProcessed.empty(), "Some files are still processed when FileDataDBManager is being deleted!");
}

ClassName FileDataDBManager::determineClassName(DataDBKey key)
{
	std::vector<std::string> splitted;
	boost::algorithm::split(splitted, key, [](char s){return s == '/' || s == '\\';});

	return splitted.at(splitted.size() - 2);
}
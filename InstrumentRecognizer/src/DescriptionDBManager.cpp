#include "DescriptionDBManager.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <locale>

namespace 
{
	const std::string DESCRIPTION_FILE_EXTENSION = ".descr";
	const char NUMBER_DELIMITER = ',';

	std::string getClassNameFromPath(const boost::filesystem::path& path)
	{
		return path.stem().string();
	}

	void writeObjectDescriptionToStream(std::ostream& stream, const ObjectDescription& desc)
	{
		auto i = desc.begin();
		// stream<< desc.begin()->real() << "," << desc.begin()->imag();
		stream << *desc.begin();
		for (i++; i != desc.end(); i++)
			//stream<<","<<i->real()<<","<<i->imag();
			stream<<","<<*i;

		stream << std::endl;
	}
		
	bool isEoL(std::string::value_type c)
	{
		return c == '\n' || c =='\r';
	}
	
	std::size_t predictLineCount(const std::string& str)
	{
		return std::count_if(str.begin(), str.end(), isEoL);
	}

	std::size_t predictElementCount(const std::string& str)
	{
		return std::count_if(str.begin(), str.end(), [](char a){return a == NUMBER_DELIMITER;});
	}
}

FileDescriptionDBManager::FileDescriptionDBManager(const boost::filesystem::path& path) : gatheringEnabled(false), descriptorsPerDecription(0), dbPath(path)
{
	IR_ASSERT(boost::filesystem::is_directory(path), "FileDescriptionDBManager expects to receive path to a directory. Received: " + path.string());
}

FileDescriptionDBManager::~FileDescriptionDBManager(void)
{
}
	
void FileDescriptionDBManager::startGathering()
{
	gatheringEnabled = true;
}

void FileDescriptionDBManager::addDescription(const ClassName& className, const ObjectDescription& newDescription)
{
	IR_ASSERT(gatheringEnabled, "Before adding description you have to start DescriptionDBManager");

	checkIfHasValidNumberOfDescriptors(newDescription);

	std::lock_guard<std::recursive_mutex> lock(descriptionManagerMutex);
	descriptions[className].push_back(newDescription);
}

void FileDescriptionDBManager::checkIfHasValidNumberOfDescriptors(const ObjectDescription& desc)
{
	if(desc.empty())
		throw DescriptionDBManager::InvalidNumberOfDescriptors("Empty object description given");

	else if(descriptorsPerDecription == 0)
		descriptorsPerDecription = desc.size();

	else if(desc.size() != descriptorsPerDecription)
		throw DescriptionDBManager::InvalidNumberOfDescriptors(std::string("Invalid number of descriptors. Expected: ") + boost::lexical_cast<std::string>(descriptorsPerDecription) + 
		    ", given: " + boost::lexical_cast<std::string>(desc.size()));
}

void FileDescriptionDBManager::finishGathering()
{
	IR_ASSERT(gatheringEnabled, "Before ending you have to start DescriptionDBManager");
	
	gatheringEnabled = false;
	saveDescriptions(dbPath);
}

void FileDescriptionDBManager::loadDescriptions(const std::string& path)
{
	loadDescriptions(boost::filesystem::path(path));
}

void FileDescriptionDBManager::saveDescriptions(const std::string& path)
{
	saveDescriptions(boost::filesystem::path(path));
}

void FileDescriptionDBManager::saveDescriptions(const boost::filesystem::path& path)
{
	std::lock_guard<std::recursive_mutex> lock(descriptionManagerMutex);
	for(auto i: descriptions)
		saveDescription(path, i.first, i.second);
}

void FileDescriptionDBManager::saveDescription(const boost::filesystem::path& dirPath, const ClassName& className, const ClassDescription& description)
{
	boost::filesystem::path filePath(dirPath);
	filePath /= className;
	filePath.replace_extension(DESCRIPTION_FILE_EXTENSION);

	std::ofstream file(filePath.string());

	if(!file.is_open())
		throw std::runtime_error("Cannot open file to write: " + filePath.string());

	for(auto i: description)
		writeObjectDescriptionToStream(file, i);

	file.close();
}
void FileDescriptionDBManager::loadDescriptions(const boost::filesystem::path& path)
{
	IR_ASSERT(boost::filesystem::is_directory(path), "FileDescriptionDBManager expects to receive path to a directory");
	
	for(auto i = boost::filesystem::directory_iterator(path); i != boost::filesystem::directory_iterator(); i++)
		if(boost::filesystem::is_regular_file(*i))
			loadDescription(i->path());
}

void FileDescriptionDBManager::loadDescription(const boost::filesystem::path& filePath)
{
	gatheringEnabled = true;

	std::ifstream file(filePath.string());
	IR_ASSERT(file.is_open(), std::string("Cannot open file to read (FileDBDescriptionManager: ") + filePath.string() + ")");

	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::string className = getClassNameFromPath(filePath);
	std::vector<std::string> lines(predictLineCount(str));
	lines = boost::split(lines, str, isEoL);

	try
	{
		for(auto line: lines)
		{
			if(!std::all_of(line.begin(), line.end(), [](char c){return isblank(c, std::locale());}))
				addDescription(className, loadObjectDescription(line));
		}
		gatheringEnabled = false;
	}
	catch(InvalidFile err)
	{		
		gatheringEnabled = false;
		throw InvalidFile(filePath.string() + ": " + err.what());
	}
	catch(boost::bad_lexical_cast err)
	{		
		gatheringEnabled = false;
		throw InvalidFile(filePath.string() + ": Input data cannot be interpreted as numeric.");
	}
}

ObjectDescription FileDescriptionDBManager::loadObjectDescription(const std::string& inputStr) const
{
	ObjectDescription entireDescription;
	std::vector<std::string> numbers(predictElementCount(inputStr));
	numbers = boost::split(numbers, inputStr, [](char c){return c == NUMBER_DELIMITER;});

	for(auto num: numbers)
		boost::algorithm::trim(num);

	if(numbers.size() % 2 != 0)
		throw InvalidFile("Input stream contains odd number of descriptors! They are not supported in current implementation. Entire input line: " + inputStr);
	
	try
	{
		auto it = numbers.begin();
		while(it < numbers.end())
		{
//			Descriptor::value_type re = boost::lexical_cast<Descriptor::value_type>(*it++);
//			Descriptor::value_type im = boost::lexical_cast<Descriptor::value_type>(*it++);
//			entireDescription.push_back(Descriptor(re, im));
			entireDescription.push_back(Descriptor(boost::lexical_cast<Descriptor>(*it++)));
		}
	}
	catch(boost::bad_lexical_cast err)
	{
		throw std::runtime_error(std::string(err.what()) + " in line: " + inputStr);
	}

	return entireDescription;
}

void FileDescriptionDBManager::reset()
{
	descriptions.clear();
}

const ClassDescriptionBase& FileDescriptionDBManager::getDescriptions() const
{
	return descriptions;
}
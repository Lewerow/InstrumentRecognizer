#include "DescriberOverlord.h"

#include "DataDBManager.h"
#include "DescriptionDBManager.h"
#include "DescriberFactory.h"

#include <future>

DescriberOverlord::DescriberOverlord(std::shared_ptr<DataDBManager> dataDBMgr, std::shared_ptr<DescriptionDBManager> descriptionDBMgr, 
									 std::shared_ptr<DescriberFactory> describerFact)
	: dataDBManager(dataDBMgr), descriptionDBManager(descriptionDBMgr), describerFactory(describerFact)
{
}

DescriberOverlord::~DescriberOverlord(void)
{
}

void DescriberOverlord::startProcessing()
{
	descriptionDBManager->startGathering();

	std::list<std::pair<DataRecord, std::future<ObjectDescription> > > descriptions;

	auto createDescription = [](std::shared_ptr<DescriberFactory> factory, const DataRecord&& record){
		std::shared_ptr<Describer> describer(factory->getDescriber());
		describer->setInputStream(&record.input);
		return std::move(std::make_pair(std::move(record), std::move(std::async(&Describer::generateObjectDescription, describer))));
	};
	
	while (dataDBManager->areRecordsAvailable())
	{
		DataRecord record = dataDBManager->take();
		descriptions.push_back(createDescription(describerFactory, std::move(record)));
	}

	for (auto& description: descriptions)
	{
		try
		{
			descriptionDBManager->addDescription(description.first.className, description.second.get());
			logFinished(description.first);
		}
		catch(DataDBManager::InvalidPathException er)
		{

		}
		catch(DataDBManager::StreamOpeningException er)
		{
			logParseError(er.what(), description.first);
		}
		catch(Describer::InputParsingError er)
		{
			logParseError(er.what(), description.first);
		}
		catch(Describer::RequiredDescriptorNotFound er)
		{

		}

		dataDBManager->release(description.first.dbKey);
	}

	descriptionDBManager->finishGathering();
}

void DescriberOverlord::logStarted(DataRecord& record)
{
}

void DescriberOverlord::logFinished(DataRecord& record)
{
}

void DescriberOverlord::logParseError(const char* errMsg, DataRecord& record)
{
}
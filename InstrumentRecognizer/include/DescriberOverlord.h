#pragma once

#include "common_defs.h"

#include <memory>

class DataDBManager;
class DescriptionDBManager;
class DescriberFactory;
class Logger;
struct DataRecord;

class DescriberOverlord
{
public:
	DescriberOverlord(std::shared_ptr<DataDBManager> dataDBMgr, std::shared_ptr<DescriptionDBManager> descriptionDBMgr, std::shared_ptr<DescriberFactory> describerFact);

	~DescriberOverlord(void);

	void startProcessing();

protected:
	std::shared_ptr<DataDBManager> dataDBManager;
	std::shared_ptr<DescriptionDBManager> descriptionDBManager;
	std::shared_ptr<DescriberFactory> describerFactory;
	
	void logStarted(DataRecord&);
	void logFinished(DataRecord&);
	void logParseError(const char* errMsg, DataRecord&);
private:
	DescriberOverlord(){} /*=delete*/;
	DescriberOverlord(const DescriberOverlord&){} /*=delete*/;
	DescriberOverlord& operator=(const DescriberOverlord&){} /*=delete*/;
};


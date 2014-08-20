#include "ClassifierObserversInitialization.h"

#include <ClassifierObservers.h>
#include <GeneralClassifierDataObserver.h>

namespace setup
{
	std::vector<ClassifierObserver*> observers(std::shared_ptr<RandomServer> random_server, std::shared_ptr<TimeServer> time_server, 
		std::shared_ptr<ReportBuilder> report_builder, int report_level, double training_data_ratio)
	{
		std::vector<ClassifierObserver*> obs;
		obs.push_back(new GeneralDataClassifierObserver);
		obs.push_back(new BoundaryClassifierObserver(time_server));
		obs.push_back(new DataCutterClassifierObserver(1 - training_data_ratio, random_server));

		for(auto& i: obs)
			i->setReportBuilder(report_builder);

		return obs;
	}
}			
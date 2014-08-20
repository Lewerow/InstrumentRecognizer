#include <vector>
#include <memory>

class ClassifierObserver;
class TimeServer;
class RandomServer;
class ReportBuilder;

namespace setup
{
	std::vector<ClassifierObserver*> observers(std::shared_ptr<RandomServer> random_server, std::shared_ptr<TimeServer> time_server, 
		std::shared_ptr<ReportBuilder> report_builder, int report_level, double training_data_ratio);
}
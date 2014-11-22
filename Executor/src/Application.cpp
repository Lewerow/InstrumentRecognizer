#include "Configuration.h"

#include <boost/program_options.hpp>	
#include <boost/optional.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <memory>
#include <vector>

// utilities
#include <RandomServer.h>
#include <TimeServer.h>

// required because of destructors
#include <ReportBuilder.h>
#include <DataDBManager.h>
#include <DescriptionDBManager.h>
#include <ClassifierDBManager.h>
#include <ReportDBManager.h>
#include <DescriberFactory.h>
#include <ClassifierFactory.h>



#include "ClassifierFactoryInitialization.h"
#include "DescriberFactoryInitialization.h"
#include "ReportBuilderInitialization.h"

#include "DataDBManagerInitialization.h"
#include "ReportDBManagerInitialization.h"
#include "ClassifierDBManagerInitialization.h"
#include "DescriptionDBManagerInitialization.h"

#include <ClassifierTesters.h>
#include <ClassifierObservers.h>
#include "ClassifierTesterInitialization.h"
#include "ClassifierObserversInitialization.h"

#include <DescriberOverlord.h>
#include <ClassifierOverlord.h>

namespace
{
	void verify_description_args(const boost::program_options::variables_map& vars)
	{
		bool was_error = false;

		if(vars.count("data_dir") == 0)
		{
			std::cerr << "Input data directory must be given (--data_dir option)" <<std::endl;
			was_error = true;
		}

		if(vars.count("description_dir") == 0)
		{
			std::cerr << "Description directory must be given (--description_dir option)" <<std::endl;
			was_error = true;
		}

		if(was_error)
			exit(-2);
	}

	void verify_classifier_args(const boost::program_options::variables_map& vars)
	{
		bool was_error = false;

		if(vars.count("description_dir") == 0)
		{
			std::cerr << "Description directory must be given (--description_dir option)" <<std::endl;
			was_error = true;
		}

		if(vars.count("classifier_dir") == 0)
		{
			std::cerr << "Classifier directory must be given (--classifier_dir option)" <<std::endl;
			was_error = true;
		}

		if(vars.count("reports_dir") == 0)
		{
			std::cerr << "Reports directory must be given (--reports_dir/-r option)" <<std::endl;
			was_error = true;
		}

		if(vars.count("classifier_type") == 0)
		{
			std::cerr << "Classifier type must be given (--classifier_type/-t option)" <<std::endl;
			was_error = true;
		}

		if(was_error)
			exit(-2);
	}	
	
	void verify_all(const boost::program_options::variables_map& vars)
	{
		verify_description_args(vars);
		verify_classifier_args(vars);
	}

}

namespace application
{	
	namespace environment
	{
		class db_managers
		{
		public:
			db_managers() : 
				data(nullptr), description(nullptr), 
				classifier(nullptr), report(nullptr)
			{}

			~db_managers(){}
			std::shared_ptr<DataDBManager> data;
			std::shared_ptr<DescriptionDBManager> description;
			std::shared_ptr<ClassifierDBManager> classifier;
			std::shared_ptr<ReportDBManager> report;
		};

		class factories
		{
		public:
			factories() : describer(nullptr), classifier(nullptr)
			{}
			~factories(){}

			std::shared_ptr<DescriberFactory> describer;
			std::shared_ptr<ClassifierFactory> classifier;
		};

		class services
		{
		public:

			services() : random(new RandomServer), time(new TimeServer)
			{}
			~services(){}

			std::shared_ptr<RandomServer> random;
			std::shared_ptr<TimeServer> time;
		};

		class builders
		{
		public:
			builders(ReportBuilder* builder = nullptr) : report(builder)
			{}
			~builders(){}

			std::shared_ptr<ReportBuilder> report;
		};

		class classifier_observers
		{
		public:
			classifier_observers(){}
			~classifier_observers(){}

			void add(ClassifierObserver* o)
			{
				observers.push_back(std::shared_ptr<ClassifierObserver>(o));
			}

			std::vector<std::shared_ptr<ClassifierObserver> > observers;
		};
	}
	
	class app::impl
	{
	public:

		impl() : db_managers(new environment::db_managers), factories(new environment::factories), services(new environment::services), builders(new environment::builders)
		{}

		std::unique_ptr<environment::db_managers> db_managers;
		std::unique_ptr<environment::factories> factories;
		std::unique_ptr<environment::services> services;
		std::unique_ptr<environment::builders> builders;
	};
				
	class describing::impl
	{
	public:
		impl() : overlord(nullptr)
		{}

		std::shared_ptr<DescriberOverlord> overlord;
	};

	class classifying::impl
	{
	public:
		impl() : observers(new environment::classifier_observers), overlord(nullptr)
		{}

		std::unique_ptr<environment::classifier_observers> observers;
		std::shared_ptr<ClassifierOverlord> overlord;
	};
	
	app::app() : pimpl(new impl)
	{}

	app::~app()
	{
	}
	
	void app::setup(const boost::program_options::variables_map& vars)
	{
		pimpl->services.reset(new environment::services);
	}


	describing::describing() : pimpl(new impl)
	{}

	describing::~describing()
	{}

	void describing::setup(const boost::program_options::variables_map& vars)
	{
		verify_description_args(vars);
		app::pimpl->factories->describer.reset(setup::describer_factory());

		if (vars.at("data_in_separate_files").as<int>() == 1)
			app::pimpl->db_managers->data.reset(setup::file_data_db_manager(vars.at("data_dir").as<std::string>(), vars.at("data_ext").as<std::string>()));
		else
			app::pimpl->db_managers->data.reset(setup::line_data_db_manager(vars.at("data_file").as<std::string>(), vars.at("class_name_column").as<int>()));

		app::pimpl->db_managers->description.reset(setup::description_db_manager_for_describing(vars.at("description_dir").as<std::string>(), 5));
	}

	void describing::run()
	{
		pimpl->overlord.reset(new DescriberOverlord(app::pimpl->db_managers->data, app::pimpl->db_managers->description, app::pimpl->factories->describer));
		pimpl->overlord->startProcessing();
	}

	classifying::classifying() : pimpl(new impl)
	{}

	classifying::~classifying()
	{}

	void classifying::setup(const boost::program_options::variables_map& vars)
	{
		verify_classifier_args(vars);
        
        std::string classifier_type = vars.at("classifier_type").as<std::string>();
        boost::algorithm::trim(classifier_type);
        app::pimpl->factories->classifier.reset(setup::classifier_factory(classifier_type, app::pimpl->services->random));
		
		app::pimpl->db_managers->description.reset(setup::description_db_manager_for_classifying(vars.at("description_dir").as<std::string>(), vars.at("fold_count").as<int>()));

		app::pimpl->db_managers->classifier.reset(setup::classifier_db_manager(vars.at("classifier_dir").as<std::string>(), app::pimpl->factories->classifier));

		app::pimpl->db_managers->report.reset(setup::report_db_manager(vars.at("reports_dir").as<std::string>()));
		app::pimpl->builders->report.reset(setup::report_builder(vars.at("report_format").as<std::string>(), app::pimpl->db_managers->report));

		pimpl->observers->add(setup::classifier_tester(app::pimpl->db_managers->description, app::pimpl->builders->report));

		std::vector<ClassifierObserver*> obs = setup::observers(app::pimpl->services->random, app::pimpl->services->time, app::pimpl->builders->report, 
			vars.at("report_level").as<unsigned int>(), vars.at("training_data_ratio").as<double>());
	
		for(auto i: obs)
			pimpl->observers->add(i);
	}
	
	void classifying::run()
	{
		pimpl->overlord.reset(new ClassifierOverlord(app::pimpl->factories->classifier, app::pimpl->db_managers->classifier, app::pimpl->db_managers->description));
		pimpl->overlord->setReportBuilder(app::pimpl->builders->report);
		pimpl->overlord->setObservers(pimpl->observers->observers);

		pimpl->overlord->teachOne();
	}

	everything::~everything()
	{}

	void everything::run()
	{
		describing::run();
		classifying::run();
	}

	void everything::setup(const boost::program_options::variables_map& vars)
	{
		verify_all(vars);
		
		app::pimpl->factories->describer.reset(setup::describer_factory());

        std::string classifier_type = vars.at("classifier_type").as<std::string>();
        boost::algorithm::trim(classifier_type);
		app::pimpl->factories->classifier.reset(setup::classifier_factory(classifier_type, app::pimpl->services->random));

		if (vars.at("data_in_separate_files").as<int>() == 1)
			app::pimpl->db_managers->data.reset(setup::file_data_db_manager(vars.at("data_dir").as<std::string>(), vars.at("data_ext").as<std::string>()));
		else
			app::pimpl->db_managers->data.reset(setup::line_data_db_manager(vars.at("data_file").as<std::string>(), vars.at("class_name_column").as<int>()));

		app::pimpl->db_managers->description.reset(setup::description_db_manager_for_describing(vars.at("description_dir").as<std::string>(), vars.at("fold_count").as<int>()));
		app::pimpl->db_managers->classifier.reset(setup::classifier_db_manager(vars.at("classifier_dir").as<std::string>(), app::pimpl->factories->classifier));
		app::pimpl->db_managers->report.reset(setup::report_db_manager(vars.at("reports_dir").as<std::string>()));

		app::pimpl->builders->report.reset(setup::report_builder(vars.at("report_format").as<std::string>(), app::pimpl->db_managers->report));

		classifying::pimpl->observers->add(setup::classifier_tester(app::pimpl->db_managers->description, app::pimpl->builders->report));
		std::vector<ClassifierObserver*> obs = setup::observers(app::pimpl->services->random, app::pimpl->services->time, app::pimpl->builders->report, 
		/*	vars.at("report_level").as<int>()*/5, vars.at("training_data_ratio").as<double>());
		
		for(auto i: obs)
			classifying::pimpl->observers->add(i);
	}

}


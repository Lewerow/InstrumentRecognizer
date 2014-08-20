#include "Configuration.h"

#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>

namespace setup
{
	namespace
	{
		const std::string version = "0.1 alpha";

		class configuration_setter
		{
		public:
			boost::program_options::variables_map setup_options(int argc, char* argv[])
			{
				setup_options_general();
				setup_options_describer();
				setup_options_classifier();
				setup_options_reports();

				namespace style = boost::program_options::command_line_style;
				int parser_style = style::default_style ^ style::allow_guessing;
				
				boost::program_options::store(boost::program_options::parse_command_line(argc, argv, all, parser_style), app_vars);

				try
				{
				boost::program_options::store(boost::program_options::parse_config_file<char>(app_vars.at("config_file").as<std::string>().c_str(), all), app_vars);
				}
				catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::program_options::reading_file> > ex)
				{
					std::cerr << "Error while reading configuration from file: " << ex.what() << "." << std::endl << "Proceeding with startup" << std::endl; 
				}
				handle_short_circuit_options();

				return app_vars;
			}

		private:
			void setup_options_general()
			{
				boost::program_options::options_description general("General options");
				general.add_options()
					("help,h", "Print help message and exit")
					("version,v", "Print version and exit")
					("config_file,c", boost::program_options::value<std::string>()->value_name("CONFIG_FILE")->default_value("config.ini"), "Config file location")
					("log_file", boost::program_options::value<std::string>()->value_name("LOG_FILE")->default_value("log.log"), "Log file location")
					("step,s", boost::program_options::value<std::string>()->value_name("STEP_NAME"), "Step to execute. Available steps: description (d), classification (c), all (a)");

				all.add(general);
			}

			void setup_options_describer()
			{
				boost::program_options::options_description describer_configuration("Describer configuration");
				describer_configuration.add_options()
					("data_dir", boost::program_options::value<std::string>(), "Input files directory")
					("data_ext", boost::program_options::value<std::string>()->default_value(".mp7"), "Input extension")
					("description_dir", boost::program_options::value<std::string>(), "Directory where descriptions are located (or to be located)");

				all.add(describer_configuration);
			}

			void setup_options_classifier()
			{
				boost::program_options::options_description classifier_configuration("Classifier configuration");
			
				classifier_configuration.add_options()
					("classifier_dir", boost::program_options::value<std::string>()->default_value("."), "Directory to put trained classifiers in")
					("classifier_type,t", boost::program_options::value<std::string>()->default_value("Linear"), "Type of classifier to use")
					("training_data_ratio,a", boost::program_options::value<double>()->default_value(1.0), "Ratio of number of descriptions used in training to total number of decriptions");

				all.add(classifier_configuration);
			}

			void setup_options_reports()
			{
				boost::program_options::options_description report_configuration("Report configuration");
				report_configuration.add_options()
					("reports_dir,r", boost::program_options::value<std::string>(), "Directory to put reports in")
					("report_format,f", boost::program_options::value<std::string>()->default_value("text"),"Format of reports (one or more of: text, xml, markdown, html separated with semicolons). \
																											Currently only text is supported")
					("report_level,l", boost::program_options::value<unsigned int>()->default_value(5), "Level of details to report. Allowed: 0 (ultra simple report) - 6 (really big report). \
																										Currently not used, but it may change anytime");

	    		all.add(report_configuration);
			}

			void handle_short_circuit_options()
			{
				if(app_vars.count("help") || app_vars.empty())
				{
					std::cout << all;
					exit(0);
				}

				if(app_vars.count("version"))
				{
					std::cout << "Current application version: " << version;
					exit(0);
				}
			}
		
			boost::program_options::options_description all;
			boost::program_options::variables_map app_vars;
		};

	
		boost::program_options::variables_map options(int argc, char* argv[])
		{
			try
			{
				boost::program_options::variables_map app_options = configuration_setter().setup_options(argc, argv);
				return app_options;
			}
			catch(boost::program_options::ambiguous_option opt)
			{
				std::cerr << "Ambigous call: " << opt.what() << "." <<std::endl << "Terminating" << std::endl;
				exit(100);
			}
			catch(boost::program_options::unknown_option opt)
			{
				std::cerr << "Unknown option: " << opt.what() << "." << std::endl << "Terminating" << std::endl;
				exit(100);
			}
			catch(boost::program_options::invalid_command_line_syntax syn)
			{
				std::cerr << "Invalid cmd syntax: " << syn.what() << "." << std::endl << "Terminating" << std::endl;
				exit(100);
			}		
			catch(boost::bad_lexical_cast cast)
			{
				std::cerr << "Error occurred: " << cast.what() << "." << std::endl << "Terminating" << std::endl;
				exit(100);
			}
			catch(...)
			{
				std::cerr << "Unknown error occurred. " << std::endl << "Terminating" << std::endl;
				exit(200);
			}
		}
	}	

	application::app* app(int argc, char* argv[])
	{
		boost::program_options::variables_map vars(options(argc, argv));
		
		if(vars.count("step") == 0)
		{
			std::cerr << "Step to perform not given" << std::endl;
			exit(-1);
		}

		std::string step = vars.at("step").as<std::string>();
		boost::algorithm::to_lower(step);

		if(step == "d" || step == "description")
		{
			application::describing* desc = new application::describing;
			desc->setup(vars);
			return desc;
		}

		if(step == "c" || step == "classifier")
		{
			application::classifying* cls = new application::classifying;
			cls->setup(vars);
			return cls;
		}

		else if(step == "a" || step == "all")
		{
			application::everything* all = new application::everything;
			all->setup(vars);
			return all;
		}

		else
		{
			std::cerr << "Unknown step to perform" << std::endl;
			exit(200);
		}
	}
}
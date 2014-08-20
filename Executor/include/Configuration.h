#include <boost/program_options.hpp>
#include <memory>

class DescriberOverlord;
class ClassifierOverlord;

namespace application
{
	namespace environment
	{
		class db_managers;
		class factories;
		class services;
		class builders;
		class classifier_observers;
	}

	class app
	{
	public:
		app();
		virtual ~app();

		virtual void setup(const boost::program_options::variables_map&);
		virtual void run() = 0;

	protected:
		class impl;
		std::unique_ptr<impl> pimpl;
	};

	class describing : virtual public app
	{
	public:		
		describing();
		virtual ~describing();

		virtual void setup(const boost::program_options::variables_map&);
		virtual void run();

	protected:
		class impl;
		std::unique_ptr<impl> pimpl;
	};

	class classifying : virtual public app
	{
	public:
		classifying();
		virtual ~classifying();

		virtual void setup(const boost::program_options::variables_map&);
		virtual void run();

	protected:
		class impl;
		std::unique_ptr<impl> pimpl;

	};

	class everything : public describing, public classifying
	{
	public:
		virtual ~everything();

		virtual void setup(const boost::program_options::variables_map&);
		virtual void run();
	};
}

namespace setup
{
	application::app* app(int argc, char* argv[]);
}
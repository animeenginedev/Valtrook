
#include <list>
#include <string>

namespace Val {
	enum LogLevel {
		INFO,
		WARNING,
		SEVERE,
		ERROR
	};


	class Logger {
	public:
		static Logger* Instance;

		Logger();
		~Logger();

		void logMessage(LogLevel level, std::string message);

		void outputLog();
	protected:
		std::string getNameForLevel(LogLevel level);

		std::list<std::string> buffer;
	};

}

#include "RegisterToScript.h"
#include <list>
#include <string>

namespace Val {
	enum LogLevel {
		INFO,
		WARNING,
		SEVERE,
		L_ERROR
	};


	class Logger : public RegisterToScript {
	public:
		static Logger* Instance;

		Logger();
		~Logger();

		void registerToScript(chaiscript::ChaiScript* script);

		void logMessage(LogLevel level, std::string message);

		void outputLog();
	protected:
		std::string getNameForLevel(LogLevel level);

		std::list<std::string> buffer;
	};

}
#include "Logger.h"

#include <iostream>

namespace Val {
	Logger* Logger::Instance = new Logger();

	Logger::Logger() {
	}

	Logger::~Logger() {
	}

	void Logger::registerToScript(chaiscript::ChaiScript* script) {
		//Log Level
		script->add_global_const(chaiscript::const_var(LogLevel::INFO), "LogLevel_INFO");
		script->add_global_const(chaiscript::const_var(LogLevel::WARNING), "LogLevel_WARNING");
		script->add_global_const(chaiscript::const_var(LogLevel::SEVERE), "LogLevel_SEVERE");
		script->add_global_const(chaiscript::const_var(LogLevel::L_ERROR), "LogLevel_ERROR");
		script->add(chaiscript::fun([](const LogLevel lhs, const LogLevel rhs) {return (lhs == rhs); }), "==");
		script->add(chaiscript::fun([](LogLevel& lhs, const LogLevel rhs) {return (lhs = rhs); }), "=");
		//Logger
		script->add(chaiscript::user_type<Logger>(), "Logger");
		script->add(chaiscript::fun(&Logger::logMessage), "logMessage");
		script->add(chaiscript::fun(&Logger::outputLog), "outputLog");
	}

	void Logger::logMessage(LogLevel level, std::string message) {
		buffer.push_back("[" + getNameForLevel(level) + "] :: " + message + "\n");
	}
	
	void Logger::outputLog() {
		std::string r = "";
		for (std::string b : buffer) {
			r += b;
		}
		printf(r.c_str());
		buffer.clear();
	}

	std::string Logger::getNameForLevel(LogLevel level) {
		switch (level) {
			case INFO:
				return "INFO";
			case WARNING:
				return "WARNING";
			case SEVERE:
				return "SEVERE";
			case L_ERROR:
				return "L_ERROR";
		}
		return "NO_LEVEL";
	}
}
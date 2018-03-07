#include "Logger.h"

#include <iostream>

namespace Val {
	Logger* Logger::Instance = new Logger();

	Logger::Logger() {
	}

	Logger::~Logger() {
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
			case ERROR:
				return "ERROR";
		}
		return "NO_LEVEL";
	}
}
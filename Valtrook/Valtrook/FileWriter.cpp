#include "FileWriter.h"

namespace Val {
	FileWriter::FileWriter(std::string path, std::string name) {
		File.open((path + name).c_str(), std::fstream::out);
	}

	FileWriter::~FileWriter() {
		if (File.is_open()) {
			File.close();
		}
	}
}
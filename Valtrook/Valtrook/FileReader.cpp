#include "FileReader.h"

namespace Val {
	FileReader::FileReader(std::string path, std::string name) {
		File.open((path + name).c_str(), std::fstream::in);
	}

	FileReader::~FileReader() {
		if (File.is_open()) {
			File.close();
		}
	}
}
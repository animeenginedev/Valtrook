#pragma once

#include <fstream>

namespace Val {
	class FileWriter {
	public:
		FileWriter(std::string path, std::string name);
		~FileWriter();

		std::ofstream File;
	};

}
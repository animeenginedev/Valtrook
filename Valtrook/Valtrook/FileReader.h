#pragma once

#include <fstream>
namespace Val {
	class FileReader {
	public:
		FileReader(std::string path, std::string name);
		~FileReader();

		std::ifstream File;
	};

}
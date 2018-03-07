#pragma once

#include <string>

#include "json.hpp"

namespace Val {
	using json = nlohmann::json;

	enum JsonFileStatus {
		FILE_CREATED,
		FILE_SUCCESS,
		FILE_FAILED
	};

	class JsonReadWriter {
	public:
		JsonReadWriter(std::string filePath, std::string fileName);
		~JsonReadWriter();

		JsonFileStatus readAllDataFromFile();
		JsonFileStatus writeAllDataToFile();

		json* getJsonData();
	private:
		std::string file;

		json jsonData;
	};

}
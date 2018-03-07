#include "JsonReadWriter.h"

#include "FileReader.h"
#include "FileWriter.h"

namespace Val {
	JsonReadWriter::JsonReadWriter(std::string filePath, std::string fileName) : file(filePath + fileName) {
	}

	JsonReadWriter::~JsonReadWriter() {
	}

	JsonFileStatus JsonReadWriter::readAllDataFromFile() {
		FileReader read(file, "");
		if (!read.File.bad()) {
			std::string data = "";
			std::string line = "";
			while (getline(read.File, line)) {
				data += line;
			}

			if (data != "") {
				jsonData = json::parse(data);

				return FILE_SUCCESS;
			}
			jsonData = json();
			return FILE_CREATED;
		}
		return FILE_FAILED;
	}

	JsonFileStatus JsonReadWriter::writeAllDataToFile() {
		FileWriter write(file, "");
		if (!write.File.bad()) {
			write.File << jsonData.dump(1);

			return FILE_SUCCESS;
		}
		return FILE_FAILED;
	}

	json * JsonReadWriter::getJsonData() {
		return &jsonData;
	}
}
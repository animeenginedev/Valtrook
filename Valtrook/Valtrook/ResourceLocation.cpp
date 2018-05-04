#include "ResourceLocation.h"

#include "RuntimeConstants.h"
#include <string>

std::string Val::ResourceLocation::getPath() const {
	return path;
}

std::string Val::ResourceLocation::getName() const {
	return name;
}

std::string Val::ResourceLocation::getExtension() const {
	return extension;
}

std::string Val::ResourceLocation::getLocation() const {
	return location;
}

#include "ResourceLocation.h"

#include "RuntimeConstants.h"
#include <string>

Val::ResourceLocation getAudioLocation(std::string name, std::string ext) {
	return Val::ResourceLocation(name, ext, Val::RuntimeConstants::Instance->SoundPath);
}
Val::ResourceLocation getTextureLocation(std::string name, std::string ext) {
	return Val::ResourceLocation(name, ext, Val::RuntimeConstants::Instance->TexturePath);
}

void Val::ResourceLocation::registerToScript(chaiscript::ChaiScript * script) {
	script->add(chaiscript::user_type<ResourceLocation>(), "ResourceLocation");

	script->add(chaiscript::constructor<ResourceLocation(const std::string&, const std::string&, const std::string&)>(), "ResourceLocation");
	script->add(chaiscript::fun(&ResourceLocation::getPath), "getPath");
	script->add(chaiscript::fun(&ResourceLocation::getName), "getName");
	script->add(chaiscript::fun(&ResourceLocation::getExtension), "getExtension");
	script->add(chaiscript::fun(&ResourceLocation::getLocation), "getLocation");


	script->add(chaiscript::fun(&getAudioLocation), "getAudioLocation");
	script->add(chaiscript::fun(&getTextureLocation), "getTextureLocation");
}

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

#pragma once

#include <string>
#include <type_traits>
#include "RegisterToScript.h"

namespace Val {
	struct ResourceLocation final {
		ResourceLocation(std::string name, std::string ext, std::string path) :
		name(std::forward<std::string>(name)), extension(std::forward<std::string>(ext)), path(std::forward<std::string>(path)),
		location(this->path + this->name + this->extension) {
		}


		void registerToScript(chaiscript::ChaiScript* script);
		std::string getPath() const;
		std::string getName() const;
		std::string getExtension() const;

		std::string getLocation() const;

		bool operator==(const ResourceLocation& rhs) const {
			return location == rhs.location;
		}
		bool operator!=(const ResourceLocation& rhs) const {
			return location != rhs.location;
		}
	protected:
		std::string path;
		std::string name;
		std::string extension;

		std::string location;
	};
}

//Resource Locations are used in unordered_maps for organizing textures.
namespace std {
	template<>
	struct hash<Val::ResourceLocation> {
		size_t operator()(const Val::ResourceLocation& rl) const {
			return hash<string>()(rl.getLocation());
		}
	};
}
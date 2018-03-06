#pragma once

#include <string>
#include <type_traits>
namespace Val {
	struct ResourceLocation final {
		template<typename Name, typename Extension, typename Path,
			std::enable_if_t<std::is_convertible<Name, std::string>::value>,
			std::enable_if_t<std::is_convertible<Extension, std::string>::value>,
			std::enable_if_t<std::is_convertible<Path, std::string>::value>  >
		ResourceLocation(Name name, Extension ext, Path path) :
		name(std::forward<Name>(name)), extension(std::forward<Extension>(ext)), path(std::forward<Path>(path)),
		location(this->path + this->name + this->extension) {
		}

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
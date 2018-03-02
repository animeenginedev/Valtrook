#pragma once

#include "Notify.h"
#include <string>

namespace Val {
	class RuntimeConstants {
	public:
		static RuntimeConstants* Instance;

		RuntimeConstants();
		~RuntimeConstants();



		//Usefull chanable constants
		Notify<std::string> WindowName = Notify<std::string>("Valtrook Engine");

		Notify<unsigned int> Window_Width = Notify<unsigned int>(1080);
		Notify<unsigned int> Window_Height = Notify<unsigned int>(720); 


		//True constants
		const std::string AssetPath = "assets\\";

		const std::string TexturePath = AssetPath + "textures\\";
		const std::string FontPath = AssetPath + "fonts\\";
		const std::string SoundPath = AssetPath + "audio\\";
		const std::string SettingPath = AssetPath + "settings\\";

		
		const unsigned int Min_Window_Width = 320; //Arbitrary numbers
		const unsigned int Min_Window_Height = 320;
	};
}
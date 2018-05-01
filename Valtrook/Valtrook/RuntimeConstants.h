#pragma once

#include "Notify.h"
#include "Colour.h"
#include <string>

namespace Val {
	class RuntimeConstants {
	public:
		static RuntimeConstants* Instance;
		
		//Usefull chanable constants

		//These are linked to any created window. (Windows can be set to ignore delegates at runtime with .SetIgnoreDelegate(true))
		Notify<std::string> WindowName = Notify<std::string>("Valtrook Engine");
		Notify<std::pair<unsigned int, unsigned int>> Window_Size = Notify<std::pair<unsigned int, unsigned int>>(std::make_pair(1080, 720));
		Notify<std::pair<unsigned int, unsigned int>> Window_Size_Min = Notify<std::pair<unsigned int, unsigned int>>(std::make_pair(320, 320));

		Notify<Colour> GLClearColour = Notify<Colour>(Colour(0, 0, 0, 255));

		//True constants
		const std::string AssetPath = "assets\\";

		const std::string ShaderPath = AssetPath + "shaders\\";
		const std::string TexturePath = AssetPath + "textures\\";
		const std::string FontPath = AssetPath + "fonts\\";
		const std::string SoundPath = AssetPath + "audio\\";
		const std::string SettingPath = AssetPath + "settings\\";
		const std::string ScriptingPath = AssetPath + "scripting\\";

		const std::string TextureSheetPath = TexturePath + "sheets\\";
	};
}
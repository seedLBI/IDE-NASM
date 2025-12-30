#ifndef COMBO_LANGUAGE_H
#define COMBO_LANGUAGE_H



#include <Windows.h>
#include <glad/glad.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <WinUser.h>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"
#include "ThirdParty/IconFontCppHeaders/IconsFontAwesome6.h"

#include "Utils/OpenGL/Texture/Texture.h"
#include "Utils/File/Utils.File.h"


class ComboLanguage {
public:
	ComboLanguage(GLFWwindow* window);
	~ComboLanguage();

	void Draw(const std::string& label);

	std::string GetCurrentLanguage();
	void SetCurrentLanguage(const std::string& lang);

private:
	GLFWwindow* window;

	Texture texture_flags;
	nlohmann::json data_text;

	std::string CurrentLanguage = "en";
	std::string GlobalLanguage = "en";

	void Init();

	struct flag_texture_data {
		ImVec2 uv0;
		ImVec2 uv1;
		ImVec2 icon_size;
	};

	flag_texture_data GetFlagData(const std::string& flag_abriv);


	bool FlagSelectableFromAtlas(const char* label,
		const flag_texture_data& flag,
		bool selected,
		ImVec2* out_size = nullptr);


};


#endif
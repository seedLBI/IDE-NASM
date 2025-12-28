#ifndef TYPE_MESSAGE_DRAW_H
#define TYPE_MESSAGE_DRAW_H

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

#include "IDE/Core/FontManager/FontManager.h"
#include "IDE/Core/Theme/Theme.h"

#include "IDE/Utils/TypeMessageConsole/TypeMessageConsole.h"

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"

#include "Utils/ImGui/ImGui.Utils.h"

class TypeMessageDraw {
public:
	TypeMessageDraw(FontManager* fontManager, nlohmann::json Data);
	~TypeMessageDraw();

	void Draw(const std::vector<TypeMessageConsole>& types);
	void InitColors(const std::vector<NamedColor>& colors);

	ImColor GetColorMSG(const TypeMessageConsole& type);

private:
	FontManager* fontManager = nullptr;
	nlohmann::json Data;

	ImColor color_type_msg_compiler;
	ImColor color_type_msg_info;
	ImColor color_type_msg_warning;
	ImColor color_type_msg_error;
	ImColor color_type_msg_panic;
	ImColor color_type_msg_fatal;
	ImColor color_type_msg_parser;
	ImColor color_type_msg_success;

	ImColor color_bold_text;
};




#endif
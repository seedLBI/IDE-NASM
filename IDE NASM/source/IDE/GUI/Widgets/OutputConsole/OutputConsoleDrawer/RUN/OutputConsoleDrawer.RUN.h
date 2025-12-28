#ifndef OUTPUT_CONSOLE_DRAWER_RUN_H
#define OUTPUT_CONSOLE_DRAWER_RUN_H

#include <nlohmann/json.hpp>
#include <fstream>

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"

#include "IDE/Utils/InfoLines/IDE/InfoLine.IDE.h"
#include "IDE/Core/Solution/Solution.h"
#include "IDE/Core/FontManager/FontManager.h"

#include "IDE/Core/Theme/Theme.h"

#include "Utils/File/Utils.File.h"
#include "Utils/ImGui/ImGui.Utils.h"

#include "IDE/GUI/Widgets/OutputConsole/TypeMessageDraw/TypeMessageDraw.h"


class OutputConsoleDrawer_RUN {
public:
	OutputConsoleDrawer_RUN(Solution* solution, FontManager* fontManager, TypeMessageDraw* typeMessageDraw, nlohmann::json SyntaxData);
	~OutputConsoleDrawer_RUN();

	void DrawLine(const int& index, std::string& line);
	void InitColors(const std::vector<NamedColor>& colors);

private:
	Solution* solution = nullptr;
	FontManager* fontManager = nullptr;
	TypeMessageDraw* typeMessageDraw = nullptr;
	nlohmann::json SyntaxData;

	ImColor color_RUN;
	ImColor color_RUN_TEXT;
	ImColor color_bold_text;

};


#endif
#ifndef OUTPUT_CONSOLE_DRAWER_IDE_H
#define OUTPUT_CONSOLE_DRAWER_IDE_H

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


class OutputConsoleDrawer_IDE {
public:
	OutputConsoleDrawer_IDE(Solution* solution, FontManager* fontManager, TypeMessageDraw* typeMessageDraw, nlohmann::json SyntaxData);
	~OutputConsoleDrawer_IDE();

	void DrawLine(const int& index, InfoLineIDE& line);
	void InitColors(const std::vector<NamedColor>& colors);

private:
	Solution* solution = nullptr;
	FontManager* fontManager = nullptr;
	TypeMessageDraw* typeMessageDraw = nullptr;
	nlohmann::json SyntaxData;

	std::string GlobalLanguage = "ru";

	ImColor color_IDE;
	ImColor color_bold_text;

};




#endif
#ifndef OUTPUT_CONSOLE_DRAWER_NASM_H
#define OUTPUT_CONSOLE_DRAWER_NASM_H


#include <nlohmann/json.hpp>
#include <fstream>

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"


#include "IDE/Tools/Compiler/NASM/Parser_OutputNasm/NASM.Parser.Output.h"
#include "IDE/Core/Solution/Solution.h"
#include "IDE/Core/FontManager/FontManager.h"

#include "IDE/Core/Theme/Theme.h"


#include "Utils/File/Utils.File.h"
#include "Utils/ImGui/ImGui.Utils.h"


#include "IDE/GUI/Widgets/OutputConsole/TypeMessageDraw/TypeMessageDraw.h"

class OutputConsoleDrawer_NASM
{
public:
	OutputConsoleDrawer_NASM(Solution* solution, FontManager* fontManager, TypeMessageDraw* typeMessageDraw, nlohmann::json SyntaxData);
	~OutputConsoleDrawer_NASM();

	bool DrawLine(const int& index, NasmOutputLine& line);
	void InitColors(const std::vector<NamedColor>& colors);

	nlohmann::json* GetSyntaxData();

private:
	Solution* solution = nullptr;
	FontManager* fontManager = nullptr;
	TypeMessageDraw* typeMessageDraw = nullptr;

	nlohmann::json SyntaxData;

	std::string GlobalLanguage = "ru";


	ImColor color_NASM;
	ImColor color_NASM_flag;
	ImColor color_bold_text;
};




#endif
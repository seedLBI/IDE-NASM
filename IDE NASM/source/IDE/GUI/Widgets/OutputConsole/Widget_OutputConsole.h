#ifndef WIDGET_OUTPUTCONSOLE_H
#define WIDGET_OUTPUTCONSOLE_H

#include "IDE/Core/Widget/IWidget.h"
#include "IDE/Core/Theme/interface/IThemeLoadable.h"
#include "IDE/Core/Solution/Solution.h"
#include "IDE/Core/BuildManager/BuildManager.h"
#include "IDE/Core/FontManager/FontManager.h"

#include "IDE/GUI/Widgets/OutputConsole/OutputConsoleDrawer/NASM/OutputConsoleDrawer.NASM.h"
#include "IDE/GUI/Widgets/OutputConsole/OutputConsoleDrawer/IDE/OutputConsoleDrawer.IDE.h"
#include "IDE/GUI/Widgets/OutputConsole//OutputConsoleDrawer/RUN/OutputConsoleDrawer.RUN.h"

#include "IDE/GUI/Widgets/OutputConsole/TypeMessageDraw/TypeMessageDraw.h"
#include "IDE/GUI/Elements/ComboLanguage/ComboLanguage.h"

#include "IDE/GUI/Widgets/TextEditor/WidgetManager_TextEditior.h"

#include <nlohmann/json.hpp>
#include <any>

class Widget_OutputConsole : public IWidget, public IThemeLoadable {
public:
	Widget_OutputConsole(GLFWwindow* window, BuildManager* buildManager, Solution* solution, FontManager* fontManager, WidgetManager_TextEditor* WidgetManagerTextEditor);
	~Widget_OutputConsole();

	void Draw() override;
	void Update() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& Data) override;

	void LoadColors() override;
	std::vector<NamedColor> GetDefaultLightColors() override;
	std::vector<NamedColor> GetDefaultDarkColors() override;


private:
	GLFWwindow* window = nullptr;
	Solution* solution = nullptr;
	BuildManager* buildManager = nullptr;
	FontManager* fontManager = nullptr;
	WidgetManager_TextEditor* WidgetManagerTextEditor = nullptr;

	OutputConsoleDrawer_NASM* nasmSyntaxHighlighter = nullptr;
	OutputConsoleDrawer_IDE*  ideSyntaxHighlighter  = nullptr;
	OutputConsoleDrawer_RUN*  runSyntaxHighlighter  = nullptr;

	TypeMessageDraw* typeMessageDraw = nullptr;

	nlohmann::json DataText;


	ComboLanguage* comboLang;
	const std::string TypeOutput_Formatted = "outputConsole.typeOutput.formatted";
	const std::string TypeOutput_Original  = "outputConsole.typeOutput.original";
	std::string CurrentTypeOutput = TypeOutput_Formatted;

	ImColor color_NASM;
	ImColor color_NASM_flag;
	ImColor color_GCC;
	ImColor color_DBG;
	ImColor color_IDE;
	ImColor color_RUN;
	ImColor color_RUN_TEXT;

	ImColor color_type_msg_compiler;
	ImColor color_type_msg_info;
	ImColor color_type_msg_warning;
	ImColor color_type_msg_error;
	ImColor color_type_msg_panic;
	ImColor color_type_msg_fatal;
	ImColor color_type_msg_parser;
	ImColor color_type_msg_success;

	ImColor color_bold_text;


	void TranslateOutput(const std::string& Language, std::vector<std::any>& lines);

};


#endif 
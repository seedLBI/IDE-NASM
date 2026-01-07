#include "Widget_OutputConsole.h"

#include "IDE/Core/LocalisationManager/LocalisationManager.h"

Widget_OutputConsole::Widget_OutputConsole(
	GLFWwindow* window,
	BuildManager* buildManager,
	Solution* solution,
	FontManager* fontManager, 
	WidgetManager_TextEditor* WidgetManagerTextEditor) :
		IWidget(       "widgetName.outputConsole"), 
		IThemeLoadable ("themeItem.outputConsole")
{

	this->buildManager = buildManager;
	this->solution = solution;
	this->fontManager = fontManager;
	this->WidgetManagerTextEditor = WidgetManagerTextEditor;
	this->window = window;


	comboLang = new ComboLanguage(window);


	if (isFileExist(L"resources\\json\\TranslateOutputConsole.json")) {
		std::ifstream ifn(L"resources\\json\\TranslateOutputConsole.json");

		try {
			DataText = nlohmann::json::parse(ifn);
		}
		catch (const nlohmann::json::parse_error& e) {
			static_cast<void>(e);
#ifdef _DEBUG
			std::cout << "ERROR  OutputConsoleDrawer_NASM::OutputConsoleDrawer_NASM()\n";
#endif // _DEBUG

		}

		ifn.close();
	}
	else {
		MessageBoxW(
		glfwGetWin32Window(window),
			(stringUTF8_to_wstring(tr("error.outputConsole.loadData")) + L" (resources\\json\\TranslateOutputConsole.json)").c_str(),
			 stringUTF8_to_wstring(tr("error.outputConsole.loadData.title")).c_str(),
			MB_ICONERROR | MB_OK);
		std::abort();
	}


	typeMessageDraw = new TypeMessageDraw(fontManager, DataText);

	nasmSyntaxHighlighter = new OutputConsoleDrawer_NASM(solution, fontManager, typeMessageDraw, DataText);
	ideSyntaxHighlighter = new OutputConsoleDrawer_IDE(solution, fontManager, typeMessageDraw, DataText);
	runSyntaxHighlighter = new OutputConsoleDrawer_RUN(solution, fontManager, typeMessageDraw, DataText);



	IThemeLoadable::InitListWord(
		{
			"color.outputConsole.selectedText", 
			"color.outputConsole.NASM", 
			"color.outputConsole.NASM.flag", 
			"color.outputConsole.GCC",
			"color.outputConsole.DBG",
			"color.outputConsole.IDE",
			"color.outputConsole.RUN",
			"color.outputConsole.RUN.TEXT",
			"color.outputConsole.info",
			"color.outputConsole.warning",
			"color.outputConsole.error",
			"color.outputConsole.parser",
			"color.outputConsole.panic",
			"color.outputConsole.fatal",
			"color.outputConsole.compiler",
			"color.outputConsole.success"
		}
	);

}

void Widget_OutputConsole::LoadColors() {
	std::string toSearch;

	static robin_hood::unordered_flat_map<std::string, ImColor*> translator = {
		{"color.outputConsole.NASM",         &color_NASM},
		{"color.outputConsole.NASM.flag",    &color_NASM_flag},
		{"color.outputConsole.selectedText", &color_bold_text},
		{"color.outputConsole.GCC",			 &color_GCC},
		{"color.outputConsole.DBG",			 &color_DBG},
		{"color.outputConsole.IDE",			 &color_IDE},
		{"color.outputConsole.RUN",			 &color_RUN},
		{"color.outputConsole.RUN.TEXT",	 &color_RUN_TEXT},
		{"color.outputConsole.compiler",	 &color_type_msg_compiler},
		{"color.outputConsole.info",		 &color_type_msg_info},
		{"color.outputConsole.warning",		 &color_type_msg_warning},
		{"color.outputConsole.error",		 &color_type_msg_error},
		{"color.outputConsole.panic",		 &color_type_msg_panic},
		{"color.outputConsole.fatal",		 &color_type_msg_fatal},
		{"color.outputConsole.parser",		 &color_type_msg_parser},
		{"color.outputConsole.success",		 &color_type_msg_success},
	};


	for (int i = 0; i < object_colors.colors.size(); i++) {

		toSearch = object_colors.colors[i].nameColor;

		if (!translator.contains(toSearch))
			std::abort();

		*translator[toSearch] = object_colors.colors[i].color;
	}

	typeMessageDraw->InitColors(object_colors.colors);
	nasmSyntaxHighlighter->InitColors(object_colors.colors);
	ideSyntaxHighlighter->InitColors(object_colors.colors);
	runSyntaxHighlighter->InitColors(object_colors.colors);
}

std::vector<NamedColor> Widget_OutputConsole::GetDefaultLightColors() {

	std::vector<NamedColor> colors = {
		{"color.outputConsole.NASM",		ImColor(0,19,255, 255)},
		{"color.outputConsole.NASM.flag",	ImColor(251,255,100,255)},
		{"color.outputConsole.selectedText",ImColor(255,0,0,255)},
		{"color.outputConsole.GCC",			ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{"color.outputConsole.DBG",			ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{"color.outputConsole.IDE",			ImColor(112, 37, 0, 255)},
		{"color.outputConsole.RUN",			ImColor(255, 0, 58, 255)},
		{"color.outputConsole.RUN.TEXT",	ImColor(11, 26, 141, 255)},
		{"color.outputConsole.info",		ImColor(0,120,185,255)},
		{"color.outputConsole.warning",		ImColor(183,106,0,255)},
		{"color.outputConsole.error",		ImColor(176,0,0,255)},
		{"color.outputConsole.panic",		ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{"color.outputConsole.fatal",		ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{"color.outputConsole.parser",		ImColor(46,147,112,255)},
		{"color.outputConsole.success",		ImColor(0,128,7,255)},
		{"color.outputConsole.compiler",	ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
	};

	return colors;
}

std::vector<NamedColor> Widget_OutputConsole::GetDefaultDarkColors() {

	std::vector<NamedColor> colors = {
		{"color.outputConsole.NASM",		ImColor(0.45f, 0.45f, 1.f, 1.f)},
		{"color.outputConsole.NASM.flag",	ImColor(255.f / 255.f, 194.f / 255.f, 3.f / 255.f, 1.f)},
		{"color.outputConsole.selectedText",ImColor(255.f / 255.f, 194.f / 255.f, 3.f / 255.f, 1.f)},
		{"color.outputConsole.GCC",			ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{"color.outputConsole.DBG",			ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{"color.outputConsole.IDE",			ImColor(241.f / 255.f, 51.f / 255.f, 255.f / 255.f, 1.f)},
		{"color.outputConsole.RUN",			ImColor(255, 40, 40, 255)},
		{"color.outputConsole.RUN.TEXT",	ImColor(255, 40, 40, 255)},
		{"color.outputConsole.info",		ImColor(100.f / 255.f, 172.f / 255.f, 255.f / 255.f, 1.00f)},
		{"color.outputConsole.warning",		ImColor(255.f / 255.f, 128.f / 255.f, 3.f / 255.f, 1.00f)},
		{"color.outputConsole.error",		ImColor(255.f / 255.f, 31.f / 255.f, 0.f / 255.f, 1.00f)},
		{"color.outputConsole.panic",		ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{"color.outputConsole.fatal",		ImColor(149.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.00f)  },
		{"color.outputConsole.parser",		ImColor(0,255,161,255)},
		{"color.outputConsole.success",		ImColor(0.20f, 1.00f, 0.20f, 1.00f)},
		{"color.outputConsole.compiler",	ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
	};

	return colors;
}


Widget_OutputConsole::~Widget_OutputConsole() {

}

void Widget_OutputConsole::TranslateOutput(const std::string& Language, std::vector<std::any>& lines) {
	if (lines.empty())
		return;

	for (size_t i = 0; i < lines.size(); i++) {

		if (lines[i].type() == typeid(InfoLineIDE)) {
			
			InfoLineIDE* info = std::any_cast<InfoLineIDE>(&lines[i]);

			if (info->CurrentLanguage == Language)
				break;

			info->Translate(Language, *nasmSyntaxHighlighter->GetSyntaxData());
		}
		else if (lines[i].type() == typeid(NasmOutputLine)) {
			NasmOutputLine* info =  std::any_cast<NasmOutputLine>(&lines[i]);

			if (info->CurrentLanguage == Language)
				break;

			info->Translate(Language, *nasmSyntaxHighlighter->GetSyntaxData());
		}

	}
}

void Widget_OutputConsole::Draw() {

	if (GetFlagShow() == false)
		return;

	if (ImGui::Begin(IWidget::GetNameTranslated().c_str(), GetPtrFlagShow())) {

		auto outputData = buildManager->GetOutputAll_Formatted();

		TranslateOutput(comboLang->GetCurrentLanguage(), outputData);
		
		ImGui::Text(u8"");
		ImGui::SameLine();
		ImGui::Text(tr("outputConsole.languageDesc").c_str());
		ImGui::SameLine();
		ImGui::Text(u8"");

		ImGui::SameLine();


		ImGui::SetNextItemWidth(ImGui::CalcTextSize("Английский               ").x);
		comboLang->Draw("");

		ImGui::SameLine();

		ImGui::Text(tr("outputConsole.typeOutput").c_str());
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::CalcTextSize("Форматированный               ").x);


		if (ImGui::BeginCombo(u8"##TYPE_OUTPUT_FILES_VIEWER", tr(CurrentTypeOutput).c_str())) {

			if (ImGui::Selectable(tr(TypeOutput_Formatted).c_str()))
				CurrentTypeOutput = TypeOutput_Formatted;
			if (CurrentTypeOutput == TypeOutput_Formatted)
				ImGui::SetItemDefaultFocus();

			if(ImGui::Selectable(tr(TypeOutput_Original).c_str()))
				CurrentTypeOutput = TypeOutput_Original;
			if (CurrentTypeOutput == TypeOutput_Original)
				ImGui::SetItemDefaultFocus();

			ImGui::EndCombo();
		}

		ImGui::SameLine(0.f,30.f);

		if (ImGui::Button(tr("outputConsole.button.clearOutput").c_str())) {
			buildManager->ClearOutput();
		}

		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal,3.f);



		if (CurrentTypeOutput == TypeOutput_Formatted) {
			ImGui::BeginTable(u8"Таблица вывода", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Reorderable);

			ImGui::TableSetupColumn(tr("outputConsole.table.from").c_str(),			ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn(tr("outputConsole.table.type").c_str(),			ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn(tr("outputConsole.table.description").c_str(),	ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn(tr("outputConsole.table.file").c_str(),			ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn(tr("outputConsole.table.line").c_str(),			ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();

			for (size_t i = 0; i < outputData.size(); i++)
			{
				if (outputData[i].type() == typeid(InfoLineIDE)) {
					InfoLineIDE* info = std::any_cast<InfoLineIDE>(&outputData[i]);
					ideSyntaxHighlighter->DrawLine((int)i, *info);
					
				}
				else if (outputData[i].type() == typeid(NasmOutputLine)) {
					NasmOutputLine* info = std::any_cast<NasmOutputLine>(&outputData[i]);
					if (nasmSyntaxHighlighter->DrawLine((int)i, *info)) {
						std::wstring path = solution->GetInfo().GetPathAbsolute() + L"\\" + stringUTF8_to_wstring(info->FileName);


						if(info->HaveError)
							WidgetManagerTextEditor->Highlight_error(path, info->PosLineCode);
						else if (info->HaveWarning)
							WidgetManagerTextEditor->Highlight_warning(path, info->PosLineCode);
						else
							WidgetManagerTextEditor->Highlight_info(path, info->PosLineCode);


						WidgetManagerTextEditor->SetActiveFromPath(path);
					}
				}
				else if (outputData[i].type() == typeid(std::string)) {
					std::string* info = std::any_cast<std::string>(&outputData[i]);
					runSyntaxHighlighter->DrawLine((int)i, *info);
				}
				
			}




			ImGui::EndTable();
		}
		else if (CurrentTypeOutput == TypeOutput_Original) {


			std::string str_TextMultiline = "";

			for (size_t i = 0; i < outputData.size(); i++) {


				if (outputData[i].type() == typeid(InfoLineIDE)) {
					InfoLineIDE* info = std::any_cast<InfoLineIDE>(&outputData[i]);
					str_TextMultiline += info->description + info->description_additional + "\n";
				}
				else if (outputData[i].type() == typeid(NasmOutputLine)) {
					NasmOutputLine* info = std::any_cast<NasmOutputLine>(&outputData[i]);
					str_TextMultiline += info->OriginalText + "\n";

				}
				else if (outputData[i].type() == typeid(std::string)) {
					std::string* info = std::any_cast<std::string>(&outputData[i]);
					str_TextMultiline += *info + "\n";
				}

			}

			ImGui::Text(str_TextMultiline.c_str());

			TooltipTranslated("outputConsole.originalOutput.tooltip");

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				ImGui::SetClipboardText(str_TextMultiline.c_str());
			}



		}
		





		ImGui::End();
	}
	else {
		ImGui::End();
	}



}

void Widget_OutputConsole::Update() {
	UpdateActive();
}



nlohmann::json Widget_OutputConsole::Save() {

	nlohmann::json result;

	result["flag_Show"] = GetFlagShow();
	result["CurrentTypeOutput"] = CurrentTypeOutput;
	result["comboLang"] = comboLang->GetCurrentLanguage();
	
	return result;
}

void Widget_OutputConsole::Load(const nlohmann::json& Data) {
	if (Data.contains("flag_Show"))
		SetFlagShow(Data["flag_Show"].get<bool>());
	if (Data.contains("CurrentTypeOutput"))
		CurrentTypeOutput = Data["CurrentTypeOutput"].get<std::string>();
	if (Data.contains("comboLang"))
		comboLang->SetCurrentLanguage(Data["comboLang"].get<std::string>());
}



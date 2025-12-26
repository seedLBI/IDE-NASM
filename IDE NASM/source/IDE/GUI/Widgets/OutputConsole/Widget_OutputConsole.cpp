#include "Widget_OutputConsole.h"


Widget_OutputConsole::Widget_OutputConsole(
	BuildManager* buildManager,
	Solution* solution,
	FontManager* fontManager, 
	WidgetManager_TextEditor* WidgetManagerTextEditor) : IWidget(u8"Вывод"), IThemeLoadable(u8"Вывод информации сборки") {
	this->buildManager = buildManager;
	this->solution = solution;
	this->fontManager = fontManager;
	this->WidgetManagerTextEditor = WidgetManagerTextEditor;


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
		MessageBoxW(0, L"А где файл переводов? (resources\\json\\TranslateOutputConsole.json)", L"Ошибка загрузки данных", MB_ICONERROR | MB_OK);
		exit(0);
	}


	typeMessageDraw = new TypeMessageDraw(fontManager, DataText);

	nasmSyntaxHighlighter = new OutputConsoleDrawer_NASM(solution, fontManager, typeMessageDraw, DataText);
	ideSyntaxHighlighter = new OutputConsoleDrawer_IDE(solution, fontManager, typeMessageDraw, DataText);
	runSyntaxHighlighter = new OutputConsoleDrawer_RUN(solution, fontManager, typeMessageDraw, DataText);



	IThemeLoadable::InitListWord(
		{
			u8"Выделенный текст", u8"NASM", u8"NASM флаг компиляции", u8"GCC",
			u8"DBG", u8"IDE", u8"RUN", u8"RUN_TEXT", u8"Информация", u8"Предупреждение",
			u8"Ошибка", u8"Парсер",u8"Аварийное завершение", u8"Критическая ошибка", 
			u8"Компилятор", u8"Успех"
		}
	);

}


void Widget_OutputConsole::LoadColors() {
	std::string toSearch;

	for (int i = 0; i < object_colors.colors.size(); i++) {

		toSearch = object_colors.colors[i].nameColor;

		if (toSearch == u8"NASM")
			color_NASM = object_colors.colors[i].color;
		else if (toSearch == u8"NASM флаг компиляции")
			color_NASM_flag = object_colors.colors[i].color;
		else if (toSearch == u8"Выделенный текст")
			color_bold_text = object_colors.colors[i].color;
		else if (toSearch == u8"GCC")
			color_GCC = object_colors.colors[i].color;
		else if (toSearch == u8"DBG")
			color_DBG = object_colors.colors[i].color;
		else if (toSearch == u8"IDE")
			color_IDE = object_colors.colors[i].color;
		else if (toSearch == u8"RUN")
			color_RUN = object_colors.colors[i].color;
		else if (toSearch == u8"RUN_TEXT")
			color_RUN = object_colors.colors[i].color;
		else if(toSearch == u8"Компилятор")
			color_type_msg_compiler = object_colors.colors[i].color;
		else if (toSearch == u8"Информация")
			color_type_msg_info = object_colors.colors[i].color;
		else if (toSearch == u8"Предупреждение")
			color_type_msg_warning = object_colors.colors[i].color;
		else if (toSearch == u8"Ошибка")
			color_type_msg_error = object_colors.colors[i].color;
		else if (toSearch == u8"Аварийное завершение")
			color_type_msg_panic = object_colors.colors[i].color;
		else if (toSearch == u8"Критическая ошибка")
			color_type_msg_fatal = object_colors.colors[i].color;
		else if (toSearch == u8"Парсер")
			color_type_msg_parser = object_colors.colors[i].color;
		else if (toSearch == u8"Успех")
			color_type_msg_parser = object_colors.colors[i].color;

	}

	typeMessageDraw->InitColors(object_colors.colors);
	nasmSyntaxHighlighter->InitColors(object_colors.colors);
	ideSyntaxHighlighter->InitColors(object_colors.colors);
	runSyntaxHighlighter->InitColors(object_colors.colors);
}

std::vector<NamedColor> Widget_OutputConsole::GetDefaultLightColors() {

	std::vector<NamedColor> colors = {
		{u8"NASM",						ImColor(0,19,255, 255)},
		{u8"NASM флаг компиляции",		ImColor(251,255,100,255)},
		{u8"Выделенный текст",			ImColor(255,0,0,255)},
		{u8"GCC",						ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{u8"DBG",						ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{u8"IDE",						ImColor(112, 37, 0, 255)},
		{u8"RUN",						ImColor(255, 0, 58, 255)},
		{u8"RUN_TEXT",					ImColor(11, 26, 141, 255)},
		{u8"Информация",				ImColor(0,120,185,255)},
		{u8"Предупреждение",			ImColor(183,106,0,255)},
		{u8"Ошибка",					ImColor(176,0,0,255)},
		{u8"Аварийное завершение",		ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{u8"Критическая ошибка",		ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{u8"Парсер",					ImColor(46,147,112,255)},
		{u8"Успех",						ImColor(0,128,7,255)},
		{u8"Компилятор",				ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
	};

	return colors;
}

std::vector<NamedColor> Widget_OutputConsole::GetDefaultDarkColors() {

	std::vector<NamedColor> colors = {
		{u8"NASM",						ImColor(0.45f, 0.45f, 1.f, 1.f)},
		{u8"NASM флаг компиляции",		ImColor(255.f / 255.f, 194.f / 255.f, 3.f / 255.f, 1.f)},
		{u8"Выделенный текст",			ImColor(255.f / 255.f, 194.f / 255.f, 3.f / 255.f, 1.f)},
		{u8"GCC",						ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{u8"DBG",						ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{u8"IDE",						ImColor(241.f / 255.f, 51.f / 255.f, 255.f / 255.f, 1.f)},
		{u8"RUN",						ImColor(255, 40, 40, 255)},
		{u8"RUN_TEXT",					ImColor(255, 40, 40, 255)},
		{u8"Информация",				ImColor(100.f / 255.f, 172.f / 255.f, 255.f / 255.f, 1.00f)},
		{u8"Предупреждение",			ImColor(255.f / 255.f, 128.f / 255.f, 3.f / 255.f, 1.00f)},
		{u8"Ошибка",					ImColor(255.f / 255.f, 31.f / 255.f, 0.f / 255.f, 1.00f)},
		{u8"Аварийное завершение",		ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		{u8"Критическая ошибка",		ImColor(149.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.00f)  },
		{u8"Парсер",					ImColor(0,255,161,255)},
		{u8"Успех",						ImColor(0.20f, 1.00f, 0.20f, 1.00f)},
		{u8"Компилятор",				ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
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

	if (ImGui::Begin(GetName_c_str(), GetPtrFlagShow())) {

		auto outputData = buildManager->GetOutputAll_Formatted();

		TranslateOutput(comboLang.GetCurrentLanguage(), outputData);
		
		ImGui::Text(u8"");
		ImGui::SameLine();
		ImGui::Text(u8"Язык описания");
		ImGui::SameLine();
		ImGui::Text(u8"");

		ImGui::SameLine();


		ImGui::SetNextItemWidth(ImGui::CalcTextSize("Английский               ").x);
		comboLang.Draw("");

		ImGui::SameLine();




		ImGui::Text(u8"Тип вывода");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::CalcTextSize("Форматированный               ").x);


		if (ImGui::BeginCombo(u8"##TYPE_OUTPUT_FILES_VIEWER", CurrentTypeOutput.c_str())) {

			if (ImGui::Selectable(TypeOutput_Formatted.c_str()))
				CurrentTypeOutput = TypeOutput_Formatted;
			if (CurrentTypeOutput == TypeOutput_Formatted)
				ImGui::SetItemDefaultFocus();

			if(ImGui::Selectable(TypeOutput_Original.c_str()))
				CurrentTypeOutput = TypeOutput_Original;
			if (CurrentTypeOutput == TypeOutput_Original)
				ImGui::SetItemDefaultFocus();

			ImGui::EndCombo();
		}

		ImGui::SameLine(0.f,30.f);

		if (ImGui::Button(u8"Очистить вывод")) {
			buildManager->ClearOutput();
		}

		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal,3.f);



		if (CurrentTypeOutput == TypeOutput_Formatted) {
			ImGui::BeginTable(u8"Таблица вывода", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Reorderable);

			ImGui::TableSetupColumn(u8"От кого", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn(u8"Тип", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn(u8"Описание", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn(u8"Файл", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn(u8"Строка", ImGuiTableColumnFlags_WidthFixed);
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
						WidgetManagerTextEditor->SetActiveFromPath( solution->GetInfo().GetPathAbsolute() + L"\\" + stringUTF8_to_wstring(info->FileName));
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
	result["comboLang"] = comboLang.GetCurrentLanguage();
	
	return result;
}

void Widget_OutputConsole::Load(const nlohmann::json& Data) {
	if (Data.contains("flag_Show"))
		SetFlagShow(Data["flag_Show"].get<bool>());
	if (Data.contains("CurrentTypeOutput"))
		CurrentTypeOutput = Data["CurrentTypeOutput"].get<std::string>();
	if (Data.contains("comboLang"))
		comboLang.SetCurrentLanguage(Data["comboLang"].get<std::string>());
}



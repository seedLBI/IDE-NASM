#include "OutputConsoleDrawer.NASM.h"
#include "IDE/Core/LocalisationManager/LocalisationManager.h"
#include <robin_hood.h>

OutputConsoleDrawer_NASM::OutputConsoleDrawer_NASM(Solution* solution, FontManager* fontManager, TypeMessageDraw* typeMessageDraw, nlohmann::json SyntaxData) {
	this->solution = solution;
	this->fontManager = fontManager;
	this->SyntaxData = SyntaxData;
	this->typeMessageDraw = typeMessageDraw;
}

nlohmann::json* OutputConsoleDrawer_NASM::GetSyntaxData() {
	return &SyntaxData;
}

OutputConsoleDrawer_NASM::~OutputConsoleDrawer_NASM() {

}


void OutputConsoleDrawer_NASM::InitColors(const std::vector<NamedColor>& colors) {


	static robin_hood::unordered_flat_map<std::string, ImColor*> translator = {
		{"color.outputConsole.NASM",         &color_NASM},
		{"color.outputConsole.NASM.flag",    &color_NASM_flag},
		{"color.outputConsole.selectedText", &color_bold_text}
	};

	for (int i = 0; i < colors.size(); i++) {

		const std::string toSearch = colors[i].nameColor;

		if (translator.contains(toSearch))
			*translator[toSearch] = colors[i].color;

	}

}

bool OutputConsoleDrawer_NASM::DrawLine(const int& index, NasmOutputLine& line) {

	bool Pressed = false;

	ImGui::TableNextRow();
	if (line.TypeNotification.empty() == false) {
		auto color_bg_row = typeMessageDraw->GetColorMSG(line.TypeNotification.front());
		color_bg_row.Value.w = 0.07f;
		ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0 + 0, color_bg_row);
	}



	ImGui::TableSetColumnIndex(0);

	ImGui::PushStyleColor(ImGuiCol_Text, color_NASM.Value);
		
		if (line.FileName.empty()) {
			ImGui::Selectable(std::string("##" + std::to_string(index) + "LINE_OUTPUT_INFO").c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_Disabled, ImVec2(0.0, 0));
		}
		else {
			if (ImGui::Selectable(std::string("##" + std::to_string(index) + "LINE_OUTPUT_INFO").c_str(), false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.0, 0))) {
				Pressed = true;
			}
		}
		ImGui::SameLine();
		RenderTextAlignedClipped("[NASM] ",0,0,0.5f);
	ImGui::PopStyleColor();

	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::Text(SyntaxData["NASM"]["description"][gl()].get<std::string>().c_str());
		ImGui::EndTooltip();
	}


	ImGui::TableSetColumnIndex(1);
	if (line.TypeNotification.empty() == false) {
		typeMessageDraw->Draw(line.TypeNotification);
	}
	else {
		ImGui::Text("");
	}



	ImGui::TableSetColumnIndex(2);



	ImGui::BeginDisabled();
	ImGui::Text(ICON_FA_COPY);
	ImGui::EndDisabled();





	ImGui::SameLine(0, 10.f);
	ImGui::Text(line.TextNotification_translated.c_str());
	if (line.FlagWarning.empty() == false) {
		ImGui::SameLine();


		ImGui::TextColored(color_NASM_flag, ("[" + line.FlagWarning + "]").c_str());


		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();


			std::string sub_flag = line.FlagWarning.substr(3);


			fontManager->Push_Bold();
				ImGui::TextColored(color_bold_text, std::string(tr("outputConsoleDrawer.NASM.flag.tooltip") + " - [" + sub_flag + "]").c_str());
			ImGui::PopFont();

			ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 3.f);



			const std::string textDesc = SyntaxData["NASM"]["TypeWarningFlag"][sub_flag]["description"][gl()].get<std::string>();
			ImGui::Text(textDesc.c_str());

			ImGui::EndTooltip();
		}

	}
	else {
		ImGui::SameLine();
		ImGui::Text("");
	}



	ImGui::TableSetColumnIndex(3);
	if (line.FileName.empty() == false) {
		std::string temp = line.FileName;

		RemoveSubstringFromStart(temp, wstring_to_stringUTF8(solution->GetInfo().GetPathSource() + L"\\"));

		RenderTextAlignedClipped(temp.c_str(), 0, 0, 0.5f);
	}
	else {
		ImGui::Text("");
	}



	ImGui::TableSetColumnIndex(4);
	if (line.PosLineCode != UINT64_MAX) {
		RenderTextAlignedClipped(std::to_string(line.PosLineCode).c_str(), 0, 0, 0.5f);
	}
	else {
		ImGui::Text("");
	}

	return Pressed;

}
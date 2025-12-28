#include "OutputConsoleDrawer.RUN.h"
#include "IDE/Core/LocalisationManager/LocalisationManager.h"
#include <robin_hood.h>

OutputConsoleDrawer_RUN::OutputConsoleDrawer_RUN(
	Solution* solution, 
	FontManager* fontManager, 
	TypeMessageDraw* typeMessageDraw, 
	nlohmann::json SyntaxData)
{
	this->solution = solution;
	this->fontManager = fontManager;
	this->SyntaxData = SyntaxData;
	this->typeMessageDraw = typeMessageDraw;
}

OutputConsoleDrawer_RUN::~OutputConsoleDrawer_RUN(){

}

void OutputConsoleDrawer_RUN::DrawLine(const int& index, std::string& line){

	ImGui::TableNextRow();
	auto color_bg_row = typeMessageDraw->GetColorMSG(TypeMessageConsole::MessageConsole_Info);
	color_bg_row.Value.w = 0.07f;
	ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0 + 0, color_bg_row);



	ImGui::TableSetColumnIndex(0);

	ImGui::PushStyleColor(ImGuiCol_Text, color_RUN.Value);
	ImGui::Selectable(std::string("##" + std::to_string(index) + "LINE_OUTPUT_INFO").c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_Disabled, ImVec2(0.0, 0));
	ImGui::SameLine();
	RenderTextAlignedClipped("[RUN] ", 0, 0, 0.5f);
	ImGui::PopStyleColor();

	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::Text(SyntaxData["RUN"]["description"][gl()].get<std::string>().c_str());
		ImGui::EndTooltip();
	}


	ImGui::TableSetColumnIndex(1);
	typeMessageDraw->Draw({ TypeMessageConsole::MessageConsole_Info });



	ImGui::TableSetColumnIndex(2);

	ImGui::BeginDisabled();
	ImGui::Text(ICON_FA_COPY);
	ImGui::EndDisabled();

	ImGui::SameLine(0, 10.f);
	ImGui::PushStyleColor(ImGuiCol_Text, color_RUN_TEXT.Value);
	ImGui::Text(line.c_str());
	ImGui::PopStyleColor();
	ImGui::SameLine(0, 0);
	ImGui::PushStyleColor(ImGuiCol_Text, color_bold_text.Value);
	ImGui::Text("");
	ImGui::PopStyleColor();



	ImGui::TableSetColumnIndex(3);
	ImGui::Text("");

	ImGui::TableSetColumnIndex(4);
	ImGui::Text("");


}

void OutputConsoleDrawer_RUN::InitColors(const std::vector<NamedColor>& colors){


	static robin_hood::unordered_flat_map<std::string, ImColor*> translator = {
		{"color.outputConsole.selectedText", &color_bold_text},
		{"color.outputConsole.RUN",			 &color_RUN},
		{"color.outputConsole.RUN.TEXT",	 &color_RUN_TEXT},
	};

	for (int i = 0; i < colors.size(); i++) {

		const std::string toSearch = colors[i].nameColor;

		if (translator.contains(toSearch))
			*translator[toSearch] = colors[i].color;

	}
}

#include "OutputConsoleDrawer.IDE.h"


OutputConsoleDrawer_IDE::OutputConsoleDrawer_IDE(Solution* solution, FontManager* fontManager, TypeMessageDraw* typeMessageDraw, nlohmann::json SyntaxData) {
	this->solution = solution;
	this->fontManager = fontManager;
	this->typeMessageDraw = typeMessageDraw;
	this->SyntaxData = SyntaxData;

}

OutputConsoleDrawer_IDE::~OutputConsoleDrawer_IDE() {

}


void OutputConsoleDrawer_IDE::DrawLine(const int& index, InfoLineIDE& line) {

	ImGui::TableNextRow();
	auto color_bg_row = typeMessageDraw->GetColorMSG(line.type);
	color_bg_row.Value.w = 0.07f;
	ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0 + 0, color_bg_row);



	ImGui::TableSetColumnIndex(0);

	ImGui::PushStyleColor(ImGuiCol_Text, color_IDE.Value);
	ImGui::Selectable(std::string("##" + std::to_string(index) + "LINE_OUTPUT_INFO").c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_Disabled, ImVec2(0.0, 0));
	ImGui::SameLine();
	RenderTextAlignedClipped("[IDE] ", 0, 0, 0.5f);
	ImGui::PopStyleColor();

	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::Text(SyntaxData["IDE"]["description"][GlobalLanguage].get<std::string>().c_str());
		ImGui::EndTooltip();
	}


	ImGui::TableSetColumnIndex(1);
	typeMessageDraw->Draw({ line.type });



	ImGui::TableSetColumnIndex(2);

	ImGui::BeginDisabled();
	ImGui::Text(ICON_FA_COPY);
	ImGui::EndDisabled();

	ImGui::SameLine(0, 10.f);
	ImGui::Text(line.description.c_str());
	ImGui::SameLine(0, 0);
	ImGui::PushStyleColor(ImGuiCol_Text, color_bold_text.Value);
		ImGui::Text(line.description_additional.c_str());
	ImGui::PopStyleColor();



	ImGui::TableSetColumnIndex(3);
	ImGui::Text("");

	ImGui::TableSetColumnIndex(4);
	ImGui::Text("");

}



void OutputConsoleDrawer_IDE::InitColors(const std::vector<NamedColor>& colors) {

	for (int i = 0; i < colors.size(); i++) {

		const std::string toSearch = colors[i].nameColor;

		if (toSearch == u8"IDE")
			color_IDE = colors[i].color;
		else if (toSearch == u8"Выделенный текст")
			color_bold_text = colors[i].color;
	}
}
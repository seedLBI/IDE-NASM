#include "TypeMessageDraw.h"


TypeMessageDraw::TypeMessageDraw(FontManager* fontManager, nlohmann::json Data) {
	this->fontManager = fontManager;
	this->Data = Data["TypeNotification"];
}

TypeMessageDraw::~TypeMessageDraw() {

}

ImColor TypeMessageDraw::GetColorMSG(const TypeMessageConsole& type) {
	switch (type) {
		case MessageConsole_Compiler: return color_type_msg_compiler;
		case MessageConsole_Info: return color_type_msg_info;
		case MessageConsole_Warning: return color_type_msg_warning;
		case MessageConsole_Error: return color_type_msg_error;
		case MessageConsole_Panic: return color_type_msg_panic;
		case MessageConsole_Parser: return color_type_msg_parser;
		case MessageConsole_Fatal: return color_type_msg_fatal;
		case MessageConsole_Success: return color_type_msg_success;
	default: return ImColor(0, 0, 0);
	}
}

void TypeMessageDraw::InitColors(const std::vector<NamedColor>& colors) {

	for (int i = 0; i < colors.size(); i++) {

		const std::string toSearch = colors[i].nameColor;

		if (toSearch == u8"Выделенный текст")
			color_bold_text = colors[i].color;
		else if (toSearch == u8"Информация")
			color_type_msg_info = colors[i].color;
		else if (toSearch == u8"Предупреждение")
			color_type_msg_warning = colors[i].color;
		else if (toSearch == u8"Ошибка")
			color_type_msg_error = colors[i].color;
		else if (toSearch == u8"Аварийное завершение")
			color_type_msg_panic = colors[i].color;
		else if (toSearch == u8"Критическая ошибка")
			color_type_msg_fatal = colors[i].color;
		else if (toSearch == u8"Парсер")
			color_type_msg_parser = colors[i].color;
		else if (toSearch == u8"Компилятор")
			color_type_msg_compiler = colors[i].color;
		else if (toSearch == u8"Успех")
			color_type_msg_success = colors[i].color;

	}
}

void TypeMessageDraw::Draw(const std::vector<TypeMessageConsole>& types) {


	if (types.size() == 1) {

		const TypeMessageConsole& type = types[0];
		const std::string& str_type = TypeMessageConsole_to_STR(type);

		const std::string& translation = Data[str_type]["translation"][GlobalLanguage].get<std::string>();
		const std::string& description = Data[str_type]["description"][GlobalLanguage].get<std::string>();

		ImGui::PushStyleColor(ImGuiCol_Text, GetColorMSG(type).Value);
		RenderTextAlignedClipped(translation.c_str(), 0, 0, 0.5f);
		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();

			fontManager->Push_Bold();
			ImGui::TextColored(color_bold_text, std::string(u8"Тип сообщения - " + translation).c_str());
			ImGui::PopFont();

			ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 3.f);

			ImGui::Text(description.c_str());

			ImGui::EndTooltip();
		}

	}
	else {
		for (size_t i = 0; i < types.size(); i++) {
			if (i != 0) {
				ImGui::SameLine();
				ImGui::Text("+");
				ImGui::SameLine();
			}

			const TypeMessageConsole& type = types[i];
			const std::string& str_type = TypeMessageConsole_to_STR(type);

			const std::string& translation = Data[str_type]["translation"][GlobalLanguage].get<std::string>();
			const std::string& description = Data[str_type]["description"][GlobalLanguage].get<std::string>();

			ImGui::PushStyleColor(ImGuiCol_Text, GetColorMSG(type).Value);
			ImGui::Text(translation.c_str(), 0, 0, 0.5f);
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();

				fontManager->Push_Bold();
				ImGui::TextColored(color_bold_text, std::string(u8"Тип сообщения - " + translation).c_str());
				ImGui::PopFont();

				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 3.f);

				ImGui::Text(description.c_str());

				ImGui::EndTooltip();
			}

		}
	}


}
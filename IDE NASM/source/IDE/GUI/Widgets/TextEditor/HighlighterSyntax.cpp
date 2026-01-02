#include "HighlighterSyntax.h"
#include <robin_hood.h>
#include "IDE/Core/LocalisationManager/LocalisationManager.h"

#include "Utils/ImGui/ImGui.Utils.h"


HighlighterSyntax::HighlighterSyntax() : IThemeLoadable("themeItem.highlighterSyntax") {
	ReadJSONFromFile();

#ifdef _DEBUG
	std::cout << MakeRegex_PreprocessorDirectives() << std::endl << std::endl;
	std::cout << MakeRegex_PredefinedMacros()		<< std::endl << std::endl;
	std::cout << MakeRegex_AssemblerDirectives()	<< std::endl << std::endl;
	std::cout << MakeRegex_Sections()				<< std::endl << std::endl;
	std::cout << MakeRegex_Instruction()			<< std::endl << std::endl;

	std::cout << MakeRegex_Register_General()		<< std::endl << std::endl;
	std::cout << MakeRegex_Register_AVX()			<< std::endl << std::endl;
	std::cout << MakeRegex_Register_Control()		<< std::endl << std::endl;
	std::cout << MakeRegex_Register_Debug()			<< std::endl << std::endl;
	std::cout << MakeRegex_Register_FloatingPoint() << std::endl << std::endl;
	std::cout << MakeRegex_Register_MMX()			<< std::endl << std::endl;
	std::cout << MakeRegex_Register_SSE()			<< std::endl << std::endl;
#endif


	IThemeLoadable::InitListWord(
		{
			"color.highlighterSyntax.instructions", 
			"color.highlighterSyntax.assemblerDirectives", 
			"color.highlighterSyntax.sections", 
			"color.highlighterSyntax.labels",
			"color.highlighterSyntax.predefinedMacros", 
			"color.highlighterSyntax.preprocessorDirectives",
			"color.highlighterSyntax.register.AVX", 
			"color.highlighterSyntax.register.MMX", 
			"color.highlighterSyntax.register.SSE",
			"color.highlighterSyntax.register.floatingPoint", 
			"color.highlighterSyntax.register.control",
			"color.highlighterSyntax.register.debug", 
			"color.highlighterSyntax.register.general"
		});

}

HighlighterSyntax::~HighlighterSyntax() {

}

void HighlighterSyntax::LoadColors() {


	robin_hood::unordered_flat_map<std::string, ImVec4*> translator = {
		{"color.highlighterSyntax.instructions",			&color_Instruction},
		{"color.highlighterSyntax.assemblerDirectives",		&color_AssemblerDirectives},
		{"color.highlighterSyntax.sections",				&color_Sections},
		{"color.highlighterSyntax.labels",					&color_Label},
		{"color.highlighterSyntax.predefinedMacros",		&color_PredefinedMacros},
		{"color.highlighterSyntax.preprocessorDirectives",	&color_PreprocessorDirectives},
		{"color.highlighterSyntax.register.AVX",			&color_Register_AVX},
		{"color.highlighterSyntax.register.MMX",			&color_Register_MMX},
		{"color.highlighterSyntax.register.SSE",			&color_Register_SSE},
		{"color.highlighterSyntax.register.floatingPoint",	&color_Register_FloatingPoint},
		{"color.highlighterSyntax.register.control",		&color_Register_Control},
		{"color.highlighterSyntax.register.debug",			&color_Register_Debug},
		{"color.highlighterSyntax.register.general",		&color_Register_General}
	};

	for (int i = 0; i < object_colors.colors.size(); i++) {

		const std::string&  name  = object_colors.colors[i].nameColor;
		const ImVec4&		color = object_colors.colors[i].color;

		if (translator.contains(name))
			*translator[name] = color;

	}

}

std::vector<NamedColor> HighlighterSyntax::GetDefaultLightColors() {
	const static std::vector<NamedColor> colors = {
		{u8"color.highlighterSyntax.labels",				ImColor(0,  0,  255, 255)},
		{u8"color.highlighterSyntax.instructions",			ImColor(25, 95, 175, 255)},
		{u8"color.highlighterSyntax.assemblerDirectives",   ImColor(0, 128, 128, 255)}, 
		{u8"color.highlighterSyntax.sections",				ImColor(106, 50, 159,255)},
		{u8"color.highlighterSyntax.predefinedMacros",		ImColor(184, 84, 0,  255)},
		{u8"color.highlighterSyntax.preprocessorDirectives",ImColor(153, 102, 51,255)},
		{u8"color.highlighterSyntax.register.AVX",          ImColor(0, 103, 119, 255)},
		{u8"color.highlighterSyntax.register.MMX",			ImColor(34, 139, 34, 255)}, 
		{u8"color.highlighterSyntax.register.SSE",			ImColor(0, 100, 102, 255)},
		{u8"color.highlighterSyntax.register.floatingPoint",ImColor(0, 100, 0, 255)},
		{u8"color.highlighterSyntax.register.control",		ImColor(128, 0, 128, 255)},
		{u8"color.highlighterSyntax.register.debug",		ImColor(139, 0, 139, 255)},
		{u8"color.highlighterSyntax.register.general",		ImColor(139, 69, 19, 255)},
	};


	return colors;
}

std::vector<NamedColor> HighlighterSyntax::GetDefaultDarkColors() {
	const static std::vector<NamedColor> colors = {
		{u8"color.highlighterSyntax.labels",				ImColor(255,255, 0,255)},
		{u8"color.highlighterSyntax.instructions",			ImColor(86, 156, 214,255)},
		{u8"color.highlighterSyntax.assemblerDirectives",	ImColor(78, 201, 176,255)},
		{u8"color.highlighterSyntax.sections",				ImColor(177, 128, 215,255)},
		{u8"color.highlighterSyntax.predefinedMacros",		ImColor(212, 158, 108,255)},
		{u8"color.highlighterSyntax.preprocessorDirectives",ImColor(220, 220, 170,255)},
		{u8"color.highlighterSyntax.register.AVX",			ImColor(86, 182, 194,255)},
		{u8"color.highlighterSyntax.register.MMX",			ImColor(165, 214, 167,255)},
		{u8"color.highlighterSyntax.register.SSE",			ImColor(78, 201, 176,255)},
		{u8"color.highlighterSyntax.register.floatingPoint",ImColor(152, 195, 121,255)},
		{u8"color.highlighterSyntax.register.control",		ImColor(198, 120, 221,255)},
		{u8"color.highlighterSyntax.register.debug",		ImColor(197, 134, 192,255)},
		{u8"color.highlighterSyntax.register.general",		ImColor(215, 186, 125,255)},
	};


	return colors;
}


HighlighterSyntax& HighlighterSyntax::Instance() {
	static HighlighterSyntax obj;
	return obj;
}

inline std::string HighlighterSyntax::MakeRegex_LowercaseAll(const nlohmann::json& array_json) {
	std::string result = "";

	result += "\\b(";

	for (auto& [key, value] : array_json.items()) {

		std::string item = key;
		ToLowerAll(item);
		result += item + "|";

	}

	result.pop_back();

	result += ")\\b";

	return result;

}
inline std::string HighlighterSyntax::MakeRegex(const nlohmann::json& array_json) {
	std::string result = "";

	result += "\\b(";

	for (auto& [key, value] : array_json.items()) {

		std::string item = key;
		result += item + "|";

	}

	result.pop_back();

	result += ")\\b";

	return result;
}

std::string HighlighterSyntax::MakeRegex_Label() {
	return "([a-zA-Z_][a-zA-Z0-9_]*)\\s*:";
}

std::string HighlighterSyntax::MakeRegex_PreprocessorDirectives() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result = MakeRegex_LowercaseAll(json_syntax["Preprocessor Directives"]);
	if (result.empty() == false)
		result.erase(result.begin(), result.begin() + 2);


	return result;
}
std::string HighlighterSyntax::MakeRegex_PredefinedMacros() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result += "\\b(";

	for (auto& [key, value] : json_syntax["Predefined Macros"].items()) {

		std::string item = key;
		size_t pos_find_question = 0;
		while (1) {

			pos_find_question = item.find_first_of('?', pos_find_question + 2);

			if (pos_find_question != std::string::npos) {
				item.insert(item.begin() + pos_find_question, '\\');
			}
			else {
				break;
			}
		}

		result += item + "|";

	}

	result.pop_back();

	result += ")\\b";

	return result;
}
std::string HighlighterSyntax::MakeRegex_AssemblerDirectives() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result = MakeRegex_LowercaseAll(json_syntax["Assembler Directives"]);

	return result;
}
std::string HighlighterSyntax::MakeRegex_Sections() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;


	result += "\\.(";

	for (auto& [key, value] : json_syntax["Default sections"].items()) {

		std::string item = key;
		item.erase(item.begin());
		ToLowerAll(item);
		result +=  item + "|";

	}

	result.pop_back();

	result += ")";



	return result;
}
std::string HighlighterSyntax::MakeRegex_Instruction() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result = MakeRegex_LowercaseAll(json_instructions);
	return result;
}

std::string HighlighterSyntax::MakeRegex_Register_General() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result = MakeRegex_LowercaseAll(json_syntax["Register"]["General-purpose"]);
	return result;
}
std::string HighlighterSyntax::MakeRegex_Register_AVX() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result = MakeRegex_LowercaseAll(json_syntax["Register"]["AVX/AVX2"]);
	return result;
}
std::string HighlighterSyntax::MakeRegex_Register_Control() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result = MakeRegex_LowercaseAll(json_syntax["Register"]["Control registers"]);
	return result;
}
std::string HighlighterSyntax::MakeRegex_Register_Debug() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result = MakeRegex_LowercaseAll(json_syntax["Register"]["Debug registers"]);
	return result;
}
std::string HighlighterSyntax::MakeRegex_Register_FloatingPoint() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result = MakeRegex_LowercaseAll(json_syntax["Register"]["Floating-point registers"]);
	return result;
}
std::string HighlighterSyntax::MakeRegex_Register_MMX() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result = MakeRegex_LowercaseAll(json_syntax["Register"]["MMX registers"]);
	return result;
}
std::string HighlighterSyntax::MakeRegex_Register_SSE() {
	static bool Created = false;
	static std::string result = "";

	if (Created)
		return result;

	Created = true;

	result = MakeRegex_LowercaseAll(json_syntax["Register"]["SSE2-SSE4"]);
	return result;
}

std::vector<std::string> HighlighterSyntax::GetAllWords() {
	static bool AlreadyCreated = false;
	static std::vector<std::string> result;
	if (AlreadyCreated)
		return result;

	for (auto& [key, value] : json_instructions.items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}
	for (auto& [key, value] : json_syntax["Preprocessor Directives"].items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}
	for (auto& [key, value] : json_syntax["Predefined Macros"].items())
		result.push_back(key);
	for (auto& [key, value] : json_syntax["Assembler Directives"].items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}
	for (auto& [key, value] : json_syntax["Default sections"].items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}

	for (auto& [key, value] : json_syntax["Register"]["General-purpose"].items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}
	for (auto& [key, value] : json_syntax["Register"]["AVX/AVX2"].items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}
	for (auto& [key, value] : json_syntax["Register"]["Control registers"].items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}
	for (auto& [key, value] : json_syntax["Register"]["Debug registers"].items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}
	for (auto& [key, value] : json_syntax["Register"]["Floating-point registers"].items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}
	for (auto& [key, value] : json_syntax["Register"]["MMX registers"].items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}
	for (auto& [key, value] : json_syntax["Register"]["SSE2-SSE4"].items()) {
		std::string item = key;
		ToLowerAll(item);
		result.push_back(item);
	}

	AlreadyCreated = true;
	return result;
}

void HighlighterSyntax::ReadJSONFromFile() {
	std::ifstream ifn;

	ifn.open("resources/json/InstructionList.json");
	json_instructions = nlohmann::json::parse(ifn);
	ifn.close();

	ifn.open("resources/json/SyntaxData.json");
	json_syntax = nlohmann::json::parse(ifn);
	ifn.close();
}



ImVec4 HighlighterSyntax::GetColorFromType(const HighlighterSyntaxType& type) {
	switch (type) {
	case Highlighter_Label:
		return color_Label;
		break;
	case Highlighter_Instruction:
		return color_Instruction;
		break;
	case Highlighter_AssemblerDirective:
		return color_AssemblerDirectives;
		break;
	case Highlighter_PreprocessorDirective:
		return color_PreprocessorDirectives;
		break;
	case Highlighter_PredefinedMacros:
		return color_PredefinedMacros;
		break;
	case Highlighter_Section:
		return color_Sections;
		break;
	case Highlighter_Register_AVX:
		return color_Register_AVX;
		break;
	case Highlighter_Register_MMX:
		return color_Register_MMX;
		break;
	case Highlighter_Register_SSE:
		return color_Register_SSE;
		break;
	case Highlighter_Register_Control:
		return color_Register_Control;
		break;
	case Highlighter_Register_Debug:
		return color_Register_Debug;
		break;
	case Highlighter_Register_FloatingPoint:
		return color_Register_FloatingPoint;
		break;
	case Highlighter_Register_General:
		return color_Register_General;
		break;
	case Highlighter_UNKNOWN:
		return ImVec4(0, 0, 0, 255);
		break;
	default:
		return ImVec4(0, 0, 0, 255);
		break;
	}
}

HighlighterSyntaxType HighlighterSyntax::GetTypeFromText(const std::string& text) {
	std::string upperCased = text;
	ToUpperALL(upperCased);

	if (json_instructions.contains(upperCased))
		return HighlighterSyntaxType::Highlighter_Instruction;
	else if (json_syntax["Default sections"].contains(upperCased))
		return HighlighterSyntaxType::Highlighter_Section;
	else if (json_syntax["Assembler Directives"].contains(upperCased))
		return HighlighterSyntaxType::Highlighter_AssemblerDirective;
	else if (json_syntax["Preprocessor Directives"].contains(upperCased))
		return HighlighterSyntaxType::Highlighter_PreprocessorDirective;
	else if (json_syntax["Predefined Macros"].contains(text))
		return HighlighterSyntaxType::Highlighter_PredefinedMacros;
	else if (json_syntax["Register"]["General-purpose"].contains(upperCased))
		return HighlighterSyntaxType::Highlighter_Register_General;
	else if (json_syntax["Register"]["AVX/AVX2"].contains(upperCased))
		return HighlighterSyntaxType::Highlighter_Register_AVX;
	else if (json_syntax["Register"]["Control registers"].contains(upperCased))
		return HighlighterSyntaxType::Highlighter_Register_Control;
	else if (json_syntax["Register"]["Debug registers"].contains(upperCased))
		return HighlighterSyntaxType::Highlighter_Register_Debug;
	else if (json_syntax["Register"]["Floating-point registers"].contains(upperCased))
		return HighlighterSyntaxType::Highlighter_Register_FloatingPoint;
	else if (json_syntax["Register"]["MMX registers"].contains(upperCased))
		return HighlighterSyntaxType::Highlighter_Register_MMX;
	else if (json_syntax["Register"]["SSE2-SSE4"].contains(upperCased))
		return HighlighterSyntaxType::Highlighter_Register_SSE;
	else if (text.back() == ':')
		return HighlighterSyntaxType::Highlighter_Label;
	else
		return HighlighterSyntaxType::Highlighter_UNKNOWN;
}


void HighlighterSyntax::DrawTooltipInstruction(const std::string& name, const nlohmann::json& json_data) {
	std::string LowerCase = name;
	ToLowerAll(LowerCase);

	ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 3.f);
	ImGui::PushStyleVar(ImGuiStyleVar_SeparatorTextBorderSize, 4.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8.0f, 4.0f));

	ImGui::SetNextWindowSizeConstraints(
		ImVec2(350.f, 0.f),
		ImVec2(FLT_MAX, FLT_MAX)
	);
		ImGui::BeginTooltip();
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // bold
			TextCenteredOnLine(name.c_str(),0,0);
			ImGui::PopFont();

			if (json_data.contains("description")){

				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // bold
					ImGui::SeparatorText(tr("highlighterSyntax.tooltip.desc").c_str());
				ImGui::PopFont();
				

				if (json_data["description"].contains(gl()))
					ImGui::TextWrapped(json_data["description"][gl()].get<std::string>().c_str());
				else if(json_data["description"].contains("en"))
					ImGui::TextWrapped(json_data["description"]["en"].get<std::string>().c_str());
				else
					ImGui::TextWrapped("ERROR NOT_HAVE_DESCRIPTION");

			}
			if (json_data.contains("operands_variants")) {

				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // bold
				ImGui::SeparatorText(tr("highlighterSyntax.tooltip.variants").c_str());
				ImGui::PopFont();

				std::vector<std::string> variant_strings;
				for (const auto& [index, variant] : json_data["operands_variants"].items()) {
					std::string operands = "";
					if (variant.contains("operands")) {
						for (const auto& ops : variant["operands"]) {
							if (operands.empty())
								operands += ops.get<std::string>();
							else
								operands += ", " + ops.get<std::string>();
						}
					}
					variant_strings.push_back(LowerCase + " " + operands);
				}
				size_t num_variants = variant_strings.size();
				if (num_variants > 0) {
					const size_t max_rows = 10;  // Maximum rows per column
					size_t num_columns = (num_variants + max_rows - 1) / max_rows;  // Ceil division
					if (num_columns < 1) num_columns = 1;
					if (ImGui::BeginTable("variants_table", static_cast<int>(num_columns), ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_BordersInnerV)) {
						for (size_t row = 0; row < max_rows; ++row) {
							ImGui::TableNextRow();
							for (size_t col = 0; col < num_columns; ++col) {
								ImGui::TableNextColumn();
								size_t idx = col * max_rows + row;
								if (idx < num_variants) {
									ImGui::Text("%s", variant_strings[idx].c_str());
								}
							}
						}
						ImGui::EndTable();
					}
				}
			}

		ImGui::EndTooltip();
	ImGui::PopStyleVar(5);
}

void HighlighterSyntax::DrawTooltipSimpleInfo(const nlohmann::json& json_data) {
	ImGui::BeginTooltip();
		ImGui::Text(json_data["description"][gl()].get<std::string>().c_str());
	ImGui::EndTooltip();
}
void HighlighterSyntax::DrawTooltipRegister(const nlohmann::json& json_data) {
	ImGui::BeginTooltip();
		ImGui::Text(json_data["description"][gl()].get<std::string>().c_str());
	ImGui::EndTooltip();
}

void HighlighterSyntax::DrawTooltip(const std::string& text) {

	HighlighterSyntaxType type = GetTypeFromText(text);
	std::string UPPERCASED_text = text;
	ToUpperALL(UPPERCASED_text);

	if (type == HighlighterSyntaxType::Highlighter_Instruction) {
		if (json_instructions.contains(UPPERCASED_text))
			DrawTooltipInstruction(UPPERCASED_text,json_instructions[UPPERCASED_text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_AssemblerDirective) {
		if (json_syntax["Assembler Directives"].contains(UPPERCASED_text))
			DrawTooltipSimpleInfo(json_syntax["Assembler Directives"][UPPERCASED_text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_PredefinedMacros) {
		if (json_syntax["Predefined Macros"].contains(text))
			DrawTooltipSimpleInfo(json_syntax["Predefined Macros"][text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_PreprocessorDirective) {
		if (json_syntax["Preprocessor Directives"].contains(UPPERCASED_text))
			DrawTooltipSimpleInfo(json_syntax["Preprocessor Directives"][UPPERCASED_text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_Section) {
		if (json_syntax["Default sections"].contains(UPPERCASED_text))
			DrawTooltipSimpleInfo(json_syntax["Default sections"][UPPERCASED_text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_Register_General) {
		if (json_syntax["Register"]["General-purpose"].contains(UPPERCASED_text))
			DrawTooltipRegister(json_syntax["Register"]["General-purpose"][UPPERCASED_text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_Register_AVX) {
		if (json_syntax["Register"]["AVX/AVX2"].contains(UPPERCASED_text))
			DrawTooltipRegister(json_syntax["Register"]["AVX/AVX2"][UPPERCASED_text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_Register_MMX) {
		if (json_syntax["Register"]["MMX registers"].contains(UPPERCASED_text))
			DrawTooltipRegister(json_syntax["Register"]["MMX registers"][UPPERCASED_text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_Register_SSE) {
		if (json_syntax["Register"]["SSE2-SSE4"].contains(UPPERCASED_text))
			DrawTooltipRegister(json_syntax["Register"]["SSE2-SSE4"][UPPERCASED_text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_Register_Control) {
		if (json_syntax["Register"]["Control registers"].contains(UPPERCASED_text))
			DrawTooltipRegister(json_syntax["Register"]["Control registers"][UPPERCASED_text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_Register_Debug) {
		if (json_syntax["Register"]["Debug registers"].contains(UPPERCASED_text))
			DrawTooltipRegister(json_syntax["Register"]["Debug registers"][UPPERCASED_text]);
	}
	else if (type == HighlighterSyntaxType::Highlighter_Register_FloatingPoint) {
		if (json_syntax["Register"]["Floating-point registers"].contains(UPPERCASED_text))
			DrawTooltipRegister(json_syntax["Register"]["Floating-point registers"][UPPERCASED_text]);
	}
}
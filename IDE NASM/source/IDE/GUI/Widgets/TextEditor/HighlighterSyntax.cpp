#include "HighlighterSyntax.h"

HighlighterSyntax::HighlighterSyntax() : IThemeLoadable(u8"Цвета инструкций и деректив") {
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
			u8"Инструкции", u8"Директивы ассемблера", u8"Секции", u8"Метки",
			u8"Предопределённые макросы", u8"Директивы препроцессора",
			u8"Регистры AVX", u8"Регистры MMX", u8"Регистры SSE",
			u8"Регистры c плавающей запятой", u8"Регистры управления",
			u8"Регистры отладки", u8"Регистры общего назначения"
		});

}

HighlighterSyntax::~HighlighterSyntax() {

}

void HighlighterSyntax::LoadColors() {
	for (int i = 0; i < object_colors.colors.size(); i++) {

		const std::string  name = object_colors.colors[i].nameColor;
		const ImVec4      color = object_colors.colors[i].color;

		if (name == u8"Метки")
			color_Label = color;
		else if (name == u8"Инструкции")
			color_Instruction				= color;
		else if (name == u8"Директивы ассемблера")
			color_AssemblerDirectives		= color;
		else if (name == u8"Секции")
			color_Sections					= color;
		else if (name == u8"Предопределённые макросы")
			color_PredefinedMacros			= color;
		else if (name == u8"Директивы препроцессора")
			color_PreprocessorDirectives	= color;
		else if (name == u8"Регистры AVX")
			color_Register_AVX				= color;
		else if (name == u8"Регистры MMX")
			color_Register_MMX				= color;
		else if (name == u8"Регистры SSE")
			color_Register_SSE				= color;
		else if (name == u8"Регистры c плавающей запятой")
			color_Register_FloatingPoint	= color;
		else if (name == u8"Регистры управления")
			color_Register_Control			= color;
		else if (name == u8"Регистры отладки")
			color_Register_Debug			= color;
		else if (name == u8"Регистры общего назначения")
			color_Register_General			= color;
	}

}

std::vector<NamedColor> HighlighterSyntax::GetDefaultLightColors() {
	const static std::vector<NamedColor> colors = {
		{u8"Метки",						 ImColor(0,  0,  255, 255)},
		{u8"Инструкции",                 ImColor(25, 95, 175, 255)},
		{u8"Директивы ассемблера",       ImColor(0, 128, 128, 255)}, 
		{u8"Секции",                     ImColor(106, 50, 159,255)},
		{u8"Предопределённые макросы",   ImColor(184, 84, 0,  255)},
		{u8"Директивы препроцессора",    ImColor(153, 102, 51,255)},
		{u8"Регистры AVX",               ImColor(0, 103, 119, 255)},
		{u8"Регистры MMX",               ImColor(34, 139, 34, 255)}, 
		{u8"Регистры SSE",               ImColor(0, 100, 102, 255)},
		{u8"Регистры c плавающей запятой", ImColor(0, 100, 0, 255)},
		{u8"Регистры управления",        ImColor(128, 0, 128, 255)},
		{u8"Регистры отладки",           ImColor(139, 0, 139, 255)},
		{u8"Регистры общего назначения", ImColor(139, 69, 19, 255)},
	};


	return colors;
}

std::vector<NamedColor> HighlighterSyntax::GetDefaultDarkColors() {
	const static std::vector<NamedColor> colors = {
		{u8"Метки",							ImColor(255,255, 0,255)},
		{u8"Инструкции",					ImColor(86, 156, 214,255)},
		{u8"Директивы ассемблера",			ImColor(78, 201, 176,255)},
		{u8"Секции",						ImColor(177, 128, 215,255)},
		{u8"Предопределённые макросы",		ImColor(212, 158, 108,255)},
		{u8"Директивы препроцессора",		ImColor(220, 220, 170,255)},
		{u8"Регистры AVX",					ImColor(86, 182, 194,255)},
		{u8"Регистры MMX",					ImColor(165, 214, 167,255)},
		{u8"Регистры SSE",					ImColor(78, 201, 176,255)},
		{u8"Регистры c плавающей запятой",	ImColor(152, 195, 121,255)},
		{u8"Регистры управления",			ImColor(198, 120, 221,255)},
		{u8"Регистры отладки",				ImColor(197, 134, 192,255)},
		{u8"Регистры общего назначения",	ImColor(215, 186, 125,255)},
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


void HighlighterSyntax::DrawTooltipInstruction(const nlohmann::json& json_data) {
	ImGui::BeginTooltip();
		ImGui::Text(json_data["description"][current_language].get<std::string>().c_str());
	ImGui::EndTooltip();
}
void HighlighterSyntax::DrawTooltipSimpleInfo(const nlohmann::json& json_data) {
	ImGui::BeginTooltip();
		ImGui::Text(json_data["description"][current_language].get<std::string>().c_str());
	ImGui::EndTooltip();
}
void HighlighterSyntax::DrawTooltipRegister(const nlohmann::json& json_data) {
	ImGui::BeginTooltip();
		ImGui::Text(json_data["description"][current_language].get<std::string>().c_str());
	ImGui::EndTooltip();
}

void HighlighterSyntax::DrawTooltip(const std::string& text) {

	HighlighterSyntaxType type = GetTypeFromText(text);
	std::string UPPERCASED_text = text;
	ToUpperALL(UPPERCASED_text);

	if (type == HighlighterSyntaxType::Highlighter_Instruction) {
		if (json_instructions.contains(UPPERCASED_text))
			DrawTooltipInstruction(json_instructions[UPPERCASED_text]);
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
#include "InfoLine.IDE.h"


std::string TypeMessageIDE_to_STR(const TypeMessageIDE& type) {
	switch (type) {
	case TypeMessageIDE::compilation_begin: return "compilation_begin";
	case TypeMessageIDE::compilation_end: return "compilation_end";
	case TypeMessageIDE::compilation_end_good: return "compilation_end_good";
	case TypeMessageIDE::compilation_end_bad_error: return "compilation_end_bad_error";
	case TypeMessageIDE::compilation_end_bad_data_wrong_section: return "compilation_end_bad_data_wrong_section";
	case TypeMessageIDE::linking_begin: return "linking_begin";
	case TypeMessageIDE::linking_end: return "linking_end";
	case TypeMessageIDE::linking_end_bad_error: return "linking_end_bad_error";
	case TypeMessageIDE::linking_end_good: return "linking_end_good";
	case TypeMessageIDE::running_begin: return "running_begin";
	case TypeMessageIDE::running_end_good: return "running_end_good";
	case TypeMessageIDE::running_end_info: return "running_end_info";
	case TypeMessageIDE::running_end_terminated: return "running_end_terminated";
	case TypeMessageIDE::running_no_exe: return "running_no_exe";
	case TypeMessageIDE::debugging_no_exe: return "debugging_no_exe";
	default: break;
	}
}


void InfoLineIDE::GetType(const nlohmann::json& dataTranslate) {
	const std::string str_type_message = TypeMessageIDE_to_STR(message_type);
	type = STR_to_TypeMessageConsole(dataTranslate[str_type_message]["type"].get<std::string>());

}
void InfoLineIDE::GetDescription(const nlohmann::json& dataTranslate) {
	const std::string str_type_message = TypeMessageIDE_to_STR(message_type);

	description =  dataTranslate[str_type_message]["translation"]["en"].get<std::string>();
}

void InfoLineIDE::Translate(const std::string& Language, const nlohmann::json& dataTranslate) {

	const std::string str_type_msg = TypeMessageIDE_to_STR(message_type);

	if (dataTranslate["IDE"]["messages"][str_type_msg]["translation"].contains(Language)) {
		description = dataTranslate["IDE"]["messages"][str_type_msg]["translation"][Language].get<std::string>();
		CurrentLanguage = Language;
	}



}
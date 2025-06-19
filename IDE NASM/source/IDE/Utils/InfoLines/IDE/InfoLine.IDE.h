#ifndef INFO_LINE_IDE
#define INFO_LINE_IDE

#include <string>
#include <nlohmann/json.hpp>
#include "IDE/Utils/TypeMessageConsole/TypeMessageConsole.h"



enum class TypeMessageIDE {
	compilation_begin,
	compilation_end,
	compilation_end_good,
	compilation_end_bad_error,
	compilation_end_bad_data_wrong_section,
	linking_begin,
	linking_end,
	linking_end_good,
	linking_end_bad_error,
	running_begin,
	running_end_good,
	running_end_info,
	running_end_terminated,
	running_no_exe,
	debugging_no_exe,
};

std::string TypeMessageIDE_to_STR(const TypeMessageIDE& type);


struct InfoLineIDE {
	TypeMessageIDE message_type;
	TypeMessageConsole type;
	std::string description;
	std::string description_additional;
	

	void GetType(const nlohmann::json& dataTranslate);
	void GetDescription(const nlohmann::json& dataTranslate);

	std::string CurrentLanguage = "en";
	void Translate(const std::string& Language, const nlohmann::json& dataTranslate);

};



#endif
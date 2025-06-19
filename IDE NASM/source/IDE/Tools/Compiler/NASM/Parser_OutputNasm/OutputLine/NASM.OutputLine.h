#ifndef NASM_OUTPUT_LINE_H
#define NASM_OUTPUT_LINE_H

#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "Utils/Text/Utils.Text.h"

#include "IDE/Utils/TypeMessageConsole/TypeMessageConsole.h"


struct NasmOutputLine {
	std::string OriginalText = "";

	std::string FileName = "";
	uint64_t	PosLineCode = UINT64_MAX;
	std::vector<TypeMessageConsole> TypeNotification;
	std::string TextNotification = ""; // original
	std::string FlagWarning = "";

	bool HaveError		= false;
	bool HaveWarning	= false;


	std::string CurrentLanguage = "en";
	std::string	TextNotification_translated = "";

	void Translate(const std::string& Language, const nlohmann::json& dataTranslate);

	void Parse(const std::string& line);
};


#endif
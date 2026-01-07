#include "NASM.OutputLine.h"


void NasmOutputLine::Parse(const std::string& line) {
    OriginalText = line;
    PosLineCode = UINT64_MAX;

    if (line.empty())
        return;

    auto IsAllNumbers = [](const std::string& text) {
        if (text.empty()) return false;
        return std::all_of(text.begin(), text.end(),
            [](char c) { return c >= '0' && c <= '9'; });
        };

    auto ExtractToken = [](std::string& str, char delim, size_t start_pos = 0) -> std::string {
        size_t pos = str.find(delim, start_pos);
        if (pos == std::string::npos) {
            std::string result = str;
            str.clear();
            return result;
        }
        std::string result = str.substr(0, pos);
        str.erase(0, pos + 1);
        return result;
        };

    std::string temp = line;
    std::string FileName, LineNumber, TypeNotification, FlagWarning;
    std::string SecondTypeNotification;


    FileName = ExtractToken(temp, ':');

    if (FileName.length() == 1 && !temp.empty()) {
        FileName += ':' + ExtractToken(temp, ':');
    }

    if (FileName == "panic") {
        TypeNotification = "panic";

        FileName = ExtractToken(temp, ':');
        EraseSideSpaces(FileName);

        if (FileName.length() == 1 && !temp.empty()) {
            FileName += ':' + ExtractToken(temp, ':');
        }

        size_t colon_pos = temp.find(':');
        if (colon_pos != std::string::npos) {
            LineNumber = temp.substr(0, colon_pos);
            if (IsAllNumbers(LineNumber)) {
                temp.erase(0, colon_pos + 1);
                EraseSideSpaces(temp);
            }
            else {
                LineNumber.clear();
            }
        }
    }
    else if (FileName == "nasm") {
        TypeNotification = ExtractToken(temp, ':');
        EraseSideSpaces(TypeNotification);
        FileName.clear();
    }
    else {
        size_t next_colon = temp.find(':');
        if (next_colon != std::string::npos) {
            LineNumber = temp.substr(0, next_colon);

            if (IsAllNumbers(LineNumber)) {
                temp.erase(0, next_colon + 1);
                EraseSideSpaces(temp);

                TypeNotification = ExtractToken(temp, ':');
                EraseSideSpaces(TypeNotification);
            }
            else {
                LineNumber.clear();
                TypeNotification = ExtractToken(temp, ':');
                EraseSideSpaces(TypeNotification);
            }
        }

        const std::vector<std::string> validTypes = {
            "fatal", "error", "warning", "info", "parser", "nasm", "panic", "..."
        };

        bool isValidType = false;
        for (const auto& type : validTypes) {
            if (TypeNotification.find(type) != std::string::npos) {
                isValidType = true;
                break;
            }
        }

        if (!isValidType) {
            if (!TypeNotification.empty()) {
                temp = TypeNotification + (temp.empty() ? "" : ":" + temp);
            }
            TypeNotification.clear();
        }

        EraseSideSpaces(temp);
        if (temp.length() >= 7 && temp.substr(0, 7) == "parser:") {
            SecondTypeNotification = "parser";
            temp.erase(0, 7);
            EraseSideSpaces(temp);
        }
    }

    size_t bracket_start = temp.find_last_of('[');
    if (bracket_start != std::string::npos) {
        size_t bracket_end = temp.find_last_of(']');
        if (bracket_end != std::string::npos && bracket_end > bracket_start) {
            if (bracket_start + 1 < temp.length() && temp[bracket_start + 1] == '-') {
                FlagWarning = temp.substr(bracket_start + 1, bracket_end - bracket_start - 1);
                temp.erase(bracket_start);
            }
        }
    }

    bool IsPanic = TypeNotification.find("panic") != std::string::npos;
    bool IsFatal = TypeNotification.find("fatal") != std::string::npos;
    bool IsError = TypeNotification.find("error") != std::string::npos;
    bool IsWarning = TypeNotification.find("warning") != std::string::npos;
    bool IsInfo = TypeNotification.find("info") != std::string::npos;

    if (IsError || IsFatal || IsPanic)
        this->HaveError = true;
    if (IsWarning)
        this->HaveWarning = true;

    EraseSideSpaces(temp);
    EraseSideSpaces(TypeNotification);
    EraseSideSpaces(FileName);

    this->FileName = FileName;

    if (!LineNumber.empty() && IsAllNumbers(LineNumber)) {
        try {
            this->PosLineCode = std::stoull(LineNumber);
        }
        catch (...) {
            this->PosLineCode = UINT64_MAX;
        }
    }

    this->TextNotification = temp;
    this->TextNotification_translated = temp;

    if (!TypeNotification.empty()) {
        this->TypeNotification.emplace_back(STR_to_TypeMessageConsole(TypeNotification));
    }

    if (!SecondTypeNotification.empty()) {
        this->TypeNotification.emplace_back(STR_to_TypeMessageConsole(SecondTypeNotification));
    }

    this->FlagWarning = FlagWarning;

}

void NasmOutputLine::Translate(const std::string& Language, const nlohmann::json& dataTranslate) {

	TextNotification_translated = TextNotification;
	CurrentLanguage = Language;

	if (Language == "en")
		return;




	for (auto& [key, value] : dataTranslate["NASM"]["TranslationOutput"].items()) {


		const std::string OriginalPartForTranslate = value["en"].get<std::string>();

		if (value.contains(CurrentLanguage) == false)
			continue;

		const std::string CustomPartForTranslate = value[CurrentLanguage].get<std::string>();


		auto pos_find = TextNotification_translated.find(OriginalPartForTranslate);

		bool Finded = pos_find != std::string::npos;

		if (Finded) {

			TextNotification_translated.replace(
				TextNotification_translated.begin() + pos_find,
				TextNotification_translated.begin() + pos_find + OriginalPartForTranslate.size(),
				CustomPartForTranslate
			);

		}
	}


}
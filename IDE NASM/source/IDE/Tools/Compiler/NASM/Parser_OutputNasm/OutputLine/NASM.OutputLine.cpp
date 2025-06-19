#include "NASM.OutputLine.h"


void NasmOutputLine::Parse(const std::string& line) {
	OriginalText = line;

	PosLineCode = UINT64_MAX;

	if (line.empty())
		return;


	auto IsAllNumbers = [](const std::string& text) {

		for (const auto& symbol : text) {
			bool isNumber = symbol >= '0' && symbol <= '9';
			if (isNumber == false)
				return false;

		}
		return true;
		};


	std::string temp = line;
	std::string FileName, PosLineCode, TypeNotification, TextNotification, FlagWarning;

	std::string SecondTypeNotification = "";

	FileName = temp.substr(0, temp.find_first_of(':'));
	if (FileName.size() == 1) {
		FileName = temp.substr(0, temp.find_first_of(':', 2));
	}
	temp.erase(temp.begin(), temp.begin() + FileName.size() + 1);


	if (FileName == "panic") {
		TypeNotification = "panic";

		FileName = temp.substr(0, temp.find_first_of(':'));
		temp.erase(temp.begin(), temp.begin() + FileName.size() + 1);

		if (FileName.size() > 0 && FileName[0] == ' ')
			FileName.erase(FileName.begin());


		PosLineCode = temp.substr(0, temp.find_first_of(':'));

		if (IsAllNumbers(PosLineCode))
			temp.erase(temp.begin(), temp.begin() + PosLineCode.size() + 2);
		else
			PosLineCode.clear();

	}
	else {
		if (FileName != "nasm") {

			PosLineCode = temp.substr(0, temp.find_first_of(':'));

			if (IsAllNumbers(PosLineCode))
				temp.erase(temp.begin(), temp.begin() + PosLineCode.size() + 2);
			else
				PosLineCode.clear();


			TypeNotification = temp.substr(0, temp.find_first_of(':'));

			std::vector<std::string> str_TypeInfo = { "fatal", "error", "warning", "info", "parser", "nasm" };
			bool ItsNotification = false;
			for (int i = 0; i < str_TypeInfo.size(); i++) {
				if (TypeNotification.find(str_TypeInfo[i]) != std::string::npos) {
					ItsNotification = true;
					break;
				}
			}

			if (ItsNotification) {
				temp.erase(temp.begin(), temp.begin() + TypeNotification.size() + 2);

				if (temp.size() > 7) {
					if (temp.substr(0, 7) == "parser:") {
						SecondTypeNotification = "parser";
						temp.erase(temp.begin(), temp.begin() + 8);
					}
				}
			}
			else {
				TypeNotification.clear();
			}


		}
		else {
			TypeNotification = FileName;
			FileName.clear();
		}


	}


	bool HaveFlag = false;

	auto posBreket = temp.find_last_of('[');

	if (posBreket != std::string::npos) {
		if (temp[posBreket + 1] == '-')
			HaveFlag = true;
	}

	if (HaveFlag) {
		FlagWarning = temp.substr(posBreket, temp.find_last_of(']') - posBreket + 1);

		FlagWarning.pop_back();
		FlagWarning.erase(FlagWarning.begin());

		temp.erase(temp.begin() + posBreket, temp.end());
	}

	bool IsPanic = TypeNotification.find("panic") != std::string::npos;
	bool IsFatal = TypeNotification.find("fatal") != std::string::npos;
	bool IsError = TypeNotification.find("error") != std::string::npos;
	bool IsWarning = TypeNotification.find("warning") != std::string::npos;
	bool IsInfo = TypeNotification.find("info") != std::string::npos;
	bool IsParser = TypeNotification.find("parser") != std::string::npos;
	bool IsNothing =
		IsPanic == false &&
		IsFatal == false &&
		IsError == false &&
		IsWarning == false &&
		IsInfo == false &&
		IsParser == false;


	if (IsError || IsFatal || IsPanic)
		this->HaveError = true;
	if (IsWarning)
		this->HaveWarning = true;


	EraseSideSpaces(TextNotification);
	EraseSideSpaces(TypeNotification);



	this->FileName = FileName;
	if (PosLineCode.empty() == false)
		this->PosLineCode = stoi(PosLineCode);
	this->TextNotification = temp;
	TextNotification_translated = this->TextNotification;

	if (TypeNotification.empty() == false){
		this->TypeNotification.emplace_back(STR_to_TypeMessageConsole(TypeNotification));
	}


	if (SecondTypeNotification.empty() == false) {
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
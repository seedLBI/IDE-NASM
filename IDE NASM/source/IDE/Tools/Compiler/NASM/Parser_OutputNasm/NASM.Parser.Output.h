#ifndef NASM_PARSER_OUTPUT_H
#define NASM_PARSER_OUTPUT_H


#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "OutputLine/NASM.OutputLine.h"

#include "Utils/File/Utils.File.h"

class ParserOutputNASM {
public:
	ParserOutputNASM();
	~ParserOutputNASM();

	void Parse(const std::vector<std::string>& lines);
	std::vector<NasmOutputLine> GetOutput();

	bool HaveWarning();
	bool HaveError();
	int GetCountErrors();

private:
	std::vector<NasmOutputLine> LastParsed;
	std::string current_lang_abriv;

	bool flag_HaveWarning = false;
	bool flag_HaveError = false;
	int countErrors = 0;

	void ClearSavedData();
};

#endif
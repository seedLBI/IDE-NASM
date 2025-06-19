#include "NASM.Parser.Output.h"

ParserOutputNASM::ParserOutputNASM() {

}
ParserOutputNASM::~ParserOutputNASM() {

}

bool ParserOutputNASM::HaveWarning() {
	return flag_HaveWarning;
}

bool ParserOutputNASM::HaveError() {
	return flag_HaveError;
}

int ParserOutputNASM::GetCountErrors() {
	return countErrors;
}

void ParserOutputNASM::Parse(const std::vector<std::string>& lines) {
	ClearSavedData();



	for (size_t i = 0; i < lines.size(); i++) {
		NasmOutputLine temp;
		temp.Parse(lines[i]);
		LastParsed.emplace_back(temp);


		if (temp.HaveWarning)
			flag_HaveWarning = true;

		if (temp.HaveError) {
			flag_HaveError = true;
			countErrors++;
		}
	}

}


std::vector<NasmOutputLine> ParserOutputNASM::GetOutput() {
	return LastParsed;
}


void ParserOutputNASM::ClearSavedData() {
	LastParsed.clear();
	flag_HaveWarning = false;
	flag_HaveError = false;
	countErrors = 0;
}
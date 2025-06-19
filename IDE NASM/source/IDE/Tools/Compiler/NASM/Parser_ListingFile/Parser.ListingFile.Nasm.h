#ifndef PARSER_LISTING_FILE_H
#define PARSER_LISTING_FILE_H

#include <vector>
#include <cstdint>
#include <string>

#include "Utils/Text/Utils.Text.h"
#include "Utils/File/Utils.File.h"
#include "Utils/Text/Encoding/UTF8/Utils.Encoding.UTF8.h"

#include <stack>
#include <basetsd.h>

enum class TypeLine {
	isNothing,
	isInclude,
	isInstruction,
	isLabel,
	isSection,
	isData,
};

struct InfoLineListing {
	uint64_t number_line_file = MAXUINT64;
	uint64_t offset_byte = MAXUINT64;
	uint64_t count_data = MAXUINT64;

	bool wrongSection = false;
	bool isSectionText = false;

	std::string file_name = "";
	std::string str_line = "";

	TypeLine type_line = TypeLine::isInstruction;
};



class ParserListingFile
{
public:
	ParserListingFile();
	~ParserListingFile();

	void Parse(const std::wstring& FileName, const std::wstring& MainFile);
	std::vector<InfoLineListing> GetOutput();

	bool HaveWarnings();


private:
	std::vector<InfoLineListing> LastParsed;
	bool flag_HaveWarnings = false;


	void ClearSavedData();
};


#endif
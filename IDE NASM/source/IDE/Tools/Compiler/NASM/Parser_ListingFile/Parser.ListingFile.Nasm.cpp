#include "Parser.ListingFile.Nasm.h"



bool ParserListingFile::HaveWarnings() {
	return flag_HaveWarnings;
}

void ParserListingFile::ClearSavedData() {
	LastParsed.clear();
	flag_HaveWarnings = false;
}

void ParserListingFile::Parse(const std::wstring& FilePath, const std::wstring& MainFile) {
	ClearSavedData();


	// Получаем строчки из Listing File
	std::vector<std::string> text = ReadTextFromFile(FilePath);



	bool have_next_line_data = false;

	std::vector<InfoLineListing> parsed_lines;


	std::string CurrentSection = "";

	std::string NextIncludeFile = "";


	int64_t CurrentDepthValue = 0;
	bool have_depth_include_value = false;


	std::stack<std::string> NameFilesCode;
	NameFilesCode.push(std::string(MainFile.begin(), MainFile.end()));


	// Парсим (жоска)
	for (int line = 0; line < text.size(); line++) {

		InfoLineListing temp_line;

		if (text[line].empty())
			continue;

		// Убераем строки с warnings и info
		if (text[line].find("******************", 0) != std::string::npos ||
			text[line].find("------------------", 0) != std::string::npos) {
			continue;
		}

		// Parse strings
		std::string parse_number_line;   // 1
		std::string parse_byte_offset;   // 2
		std::string prase_byte_data;     // 3
		std::string parse_depth_include; // 4
		std::string parse_text_code;	 // 5


		// 1
		parse_number_line = text[line].substr(0, 7);
		text[line].erase(text[line].begin(), text[line].begin() + 7);

		// 2
		parse_byte_offset = text[line].substr(0, 9);
		text[line].erase(text[line].begin(), text[line].begin() + 9);

		// 3
		prase_byte_data = text[line].substr(0, 19);
		text[line].erase(text[line].begin(), text[line].begin() + 19);


		// 4
		auto pos_left_depth = text[line].find_first_of('<');
		if (pos_left_depth <= 2) {
			auto pos_right_depth = text[line].find_first_of('>');
			parse_depth_include = text[line].substr(0, pos_right_depth + 1);
			text[line].erase(text[line].begin(), text[line].begin() + pos_right_depth + 1);
		}


		// 5
		parse_text_code = text[line];
		if (parse_text_code.empty() == false)
			parse_text_code.erase(parse_text_code.begin());



		//отчищаем от пробелов
		EraseAllSymbolType(parse_number_line, ' ');
		EraseAllSymbolType(parse_byte_offset, ' ');
		EraseAllSymbolType(prase_byte_data, ' ');
		EraseAllSymbolType(parse_depth_include, ' ');




		// Определяю является ли эта строчка относящейся к предыдущей в плане объяснения байтов данных,
		// если это так то добавляю кол-во занимаемых байтов к предыдущей строчке
		bool needContinue = false;

		if (have_next_line_data) {

			parsed_lines.back().count_data += prase_byte_data.size() / 2;

			have_next_line_data = false;

			needContinue = true;
		}

		if (prase_byte_data.empty() == false) {
			have_next_line_data = prase_byte_data.back() == '-';
			if (have_next_line_data)
				prase_byte_data.pop_back();
		}

		if (needContinue)
			continue;







		// Заполняем данные о линии
		if (parse_byte_offset.size() > 0) {
			temp_line.offset_byte = strtol(parse_byte_offset.c_str(), NULL, 16);
		}

		if (prase_byte_data.size() > 0)
			temp_line.count_data = prase_byte_data.size() / 2;

		temp_line.number_line_file = stoi(parse_number_line);

		temp_line.str_line = parse_text_code;




		int New_value_Depth = 0;

		if (parse_depth_include.empty() == false) {

			parse_depth_include.pop_back();
			parse_depth_include.erase(parse_depth_include.begin());

			New_value_Depth = stoi(parse_depth_include);
		}


		if (CurrentDepthValue > New_value_Depth) {

			int count_pop = (int(NameFilesCode.size()) - 1) - New_value_Depth;

			for (int i = 0; i < count_pop; i++) {
				NameFilesCode.pop();
			}
		}

		CurrentDepthValue = New_value_Depth;


		temp_line.file_name = NameFilesCode.top();


		std::string temp_text_code = parse_text_code;

		ReplaceAll(temp_text_code, "\t", "    ");
		DeleteAllFromFirstFindSymbol(temp_text_code, ';');
		SaveUniqueSpaces(temp_text_code);
		EraseSideSpaces(temp_text_code);


		if (temp_text_code.empty() && (parse_byte_offset.empty() || have_next_line_data == false))
			continue;



		ToLowerAll(temp_text_code);

		auto section_pos = temp_text_code.find("section");

		if (section_pos == 0)
		{
			temp_line.type_line = TypeLine::isSection;
			temp_text_code.erase(temp_text_code.begin(), temp_text_code.begin() + 7);
			EraseSideSpaces(temp_text_code);

			CurrentSection = temp_text_code;



		}
		else if (temp_text_code[0] == '%') {
			auto include_pos = temp_text_code.find("%include", 0);

			if (include_pos == 0) {
				temp_line.type_line = TypeLine::isInclude;

				temp_text_code.erase(temp_text_code.begin(), temp_text_code.begin() + 8);
				EraseSideSpaces(temp_text_code);


				temp_text_code.erase(temp_text_code.begin());
				temp_text_code.pop_back();

				NextIncludeFile = temp_text_code;
			}
		}
		else if (temp_text_code.back() == ':') {
			temp_line.type_line = TypeLine::isLabel;
		}
		else {

			// Удаляем ковычки из строки в которой возможно есть db dw dd dq и т.д
			// Зачем? затем чтобы было проще определить есть ли в строчке db dw dd dq и т.д. Иначе есть шанс найти db dw dd в ковычках - что не надо.
			while (1) {
				auto kovichki_double_pos = temp_text_code.find_first_of('\"');
				auto kovichki_one_pos = temp_text_code.find_first_of('\'');

				if (kovichki_double_pos != std::string::npos) {

					temp_text_code.erase(
						temp_text_code.begin() + kovichki_double_pos,
						temp_text_code.begin() + temp_text_code.find_first_of('\"', kovichki_double_pos + 1) + 1);

				}
				else if (kovichki_one_pos != std::string::npos) {

					temp_text_code.erase(
						temp_text_code.begin() + kovichki_one_pos,
						temp_text_code.begin() + temp_text_code.find_first_of('\'', kovichki_one_pos + 1) + 1);

				}
				else {
					break;
				}
			}

			std::vector<std::string> data_derectives = { "db","dw","dd","dq","do","dy","dz" };
			for (int i = 0; i < data_derectives.size(); i++) {
				auto pos_find = temp_text_code.find(data_derectives[i]);

				if (pos_find != std::string::npos)
				{
					if (pos_find > 0) {
						if (temp_text_code[pos_find - 1] != ' ' &&
							temp_text_code[pos_find - 1] != ':') {
							continue;
						}
					}
					if (temp_text_code[pos_find + 2] != ' ' &&
						temp_text_code[pos_find + 2] != ',') {
						continue;
					}

					if (CurrentSection != ".data" && CurrentSection != ".bss") {
						temp_line.wrongSection = true;
						flag_HaveWarnings = true;
					}

					temp_line.type_line = TypeLine::isData;
					break;
				}

			}
		}


		if (temp_line.offset_byte == UINT64_MAX && temp_line.count_data == UINT64_MAX && temp_line.type_line == TypeLine::isInstruction)
		{
			temp_line.type_line = TypeLine::isNothing;
		}


		if (CurrentSection == ".text") {
			temp_line.isSectionText = true;
		}

		//std::cout << "[" << temp_text_code << "]\n";

		parsed_lines.push_back(temp_line);


		if (NextIncludeFile.empty() == false) {
			NameFilesCode.push(NextIncludeFile);
			NextIncludeFile.clear();
		}

	}


	LastParsed = parsed_lines;

	// std::cout << line.file_name << ": " << line.number_line_file << ": Предупреждение: При определении данных в секции .text будет отключена возможность отладки." << std::endl;

}

std::vector<InfoLineListing> ParserListingFile::GetOutput() {
	return LastParsed;
}

ParserListingFile::ParserListingFile() {

}

ParserListingFile::~ParserListingFile() {

}


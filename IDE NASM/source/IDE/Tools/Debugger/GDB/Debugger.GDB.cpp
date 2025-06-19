#include "Debugger.GDB.h"

void Debugger_GDB::SetAdressToSectionText(const uint64_t& begin, const uint64_t& end) {
	section_text_begin_adress = begin;
	section_text_end_adress = end;
}

void Debugger_GDB::ClearData() {
	SetAdressToSectionText(0, 0);
	offsets_bytes.clear();
}



void Debugger_GDB::SendCommand(const std::wstring& command) {
	DebuggerRunner->SetInput(command);
}

std::vector<std::string> Debugger_GDB::WaitAnswerByKeyWord(const std::string& keyword) {
	std::vector<std::string> output;

	while (1) {
		auto data = DebuggerRunner->GetOutput();

		if (data.empty())
			continue;

		for (int i = 0; i < data.size(); i++) {
			if (data[i].empty())
				continue;
			data[i].pop_back();
#ifdef _DEBUG
			//std::cout << data[i] << std::endl;
#endif // _DEBUG
		}




		if (data.empty() == false)
			output.insert(output.end(), data.begin(), data.end());

		if (output.back() == keyword) {
			break;
		}

		Sleep(1);
	}

	output.pop_back();

	return output;
}
std::vector<std::string> Debugger_GDB::WaitAnswerByFindKeywordOnLastLine(const std::string& keyword) {
	std::vector<std::string> output;

	while (1) {
		auto data = DebuggerRunner->GetOutput();

		if (data.empty())
			continue;

		for (int i = 0; i < data.size(); i++) {
			if (data[i].empty())
				continue;
			data[i].pop_back();
		}


		if (data.empty() == false)
			output.insert(output.end(), data.begin(), data.end());

		if (output.back().find(keyword) != std::string::npos) {
			break;
		}

		Sleep(1);
	}

	//output.pop_back();

	return output;
}

std::vector<std::string> Debugger_GDB::SendCommandAndWaitAnswerByKeyWord(const std::wstring& command, const std::string& keyword) {
	SendCommand(command);
	return WaitAnswerByKeyWord(keyword);
}

void Debugger_GDB::WaitForExecAsync_Stopped() {
	while (1) {
		auto data = DebuggerRunner->GetOutput();

		if (data.empty())
			continue;

		for (int i = 0; i < data.size(); i++) {
			if (data[i].empty())
				continue;
			data[i].pop_back();

#ifdef _DEBUG
			//std::cout << data[i] << std::endl;
#endif // _DEBUG

		}

		std::string str_parse;
		for (int i = 0; i < data.size(); i++) {
			str_parse += data[i];
			str_parse += "\n";
		}




		if (data.empty() == false) {

			auto parsed = Parser.parse(str_parse);

			bool finded = false;

			for (auto& [key, value] : parsed.items()) {

				if (value.contains("record_type")) {

					bool correct_type = value["record_type"].get<std::string>() == "exec-async";

					if (correct_type) {

						bool correct_class = value["payload"]["class"].get<std::string>() == "stopped";

						if (correct_class) {
							finded = true;
							break;
						}
					}


				}
			}

			if (finded)
				break;

		}

		Sleep(1);
	}
}


void Debugger_GDB::SetAllBreakpoints() {
	std::string str_addr;

	for (auto& [key, value] : json_dissasembly["addr"].items()) {

		if (value.contains("File")) {
			SendCommand(L"-break-insert *" + stringUTF8_to_wstring(key) + L"\n");
		}
		Sleep(1);
	}


	while (1) {

		auto data = DebuggerRunner->GetOutput();

		if (data.empty())
			continue;

		for (int i = 0; i < data.size(); i++) {
			if (data[i].empty())
				continue;
			data[i].pop_back();

#ifdef _DEBUG
			//std::cout << data[i] << std::endl;
#endif // _DEBUG

		}

		std::string str_parse;
		for (int i = 0; i < data.size(); i++) {
			str_parse += data[i];
			str_parse += "\n";
		}


		if (data.empty() == false) {

			bool finded = false;

			auto parsed = Parser.parse(str_parse);

			for (auto& [key, value] : parsed.items()) {

				if (value.contains("record_type")) {

					bool correct_type = value["record_type"].get<std::string>() == "result";

					if (correct_type) {

						bool correct_class = value["payload"]["class"].get<std::string>() == "done";

						if (correct_class) {
							finded = true;
							break;
						}
					}


				}
			}

			if (finded)
				break;

		}

		Sleep(1);
	}


}

void Debugger_GDB::DeleateAllBreakpoints() {
	SendCommandAndWaitAnswerByKeyWord(L"-break-delete\n", "(gdb) ");
}



void Debugger_GDB::SetEntryPoint(const std::wstring& NameEntryPoint) {
	this->NameEntryPoint = NameEntryPoint;
}

void Debugger_GDB::SetListingInfo(const std::vector<InfoLineListing>& ListingFileInfo) {
	this->ListingFileInfo = ListingFileInfo;
}

void Debugger_GDB::Debug(const std::wstring& path2debug) {

	ClearData();

	DebuggerRunner->Run(L"--interpreter=mi ");

	Sleep(100);

	WaitAnswerByFindKeywordOnLastLine("(gdb) ");


	SendCommandAndWaitAnswerByKeyWord(L"-file-exec-and-symbols \"" + path2debug + L"\"\n", "(gdb) ");
	SendCommandAndWaitAnswerByKeyWord(L"set mi-async\n", "(gdb) ");
	SendCommandAndWaitAnswerByKeyWord(L"-break-insert " + NameEntryPoint + L"\n", "(gdb) ");

	ParseRangesTextSection();
	ParseDissasembly();
	MakeJSON_CodeWithAdresses();




	SendCommand(L"start\n");
	WaitForExecAsync_Stopped();

	SetAllBreakpoints();

	SendCommand(L"-exec-continue\n");
	WaitForExecAsync_Stopped();




	DeleateAllBreakpoints();

	ParseRegisters();

}

nlohmann::json Debugger_GDB::GetFirstParse() {
	Sleep(1);
	while (1) {

		auto data = DebuggerRunner->GetOutput();

		if (data.empty())
			continue;

		for (int i = 0; i < data.size(); i++) {
			if (data[i].empty())
				continue;
			data[i].pop_back();

#ifdef _DEBUG
			//std::cout << data[i] << std::endl;
#endif // _DEBUG

		}

		std::string str_parse;
		for (int i = 0; i < data.size(); i++) {
			str_parse += data[i];
			str_parse += "\n";
		}


		if (data.empty() == false) {
			auto parsed = Parser.parse(str_parse);
			return parsed;
		}

		Sleep(1);
	}
}


nlohmann::json Debugger_GDB::GetRegisters() {
	return json_registers;
}

void Debugger_GDB::ParseRegisters() {
	std::vector<std::string> output = SendCommandAndWaitAnswerByKeyWord(L"info registers all\n", "(gdb) ");
	

	std::string str_parse;
	for (int i = 0; i < output.size(); i++) {
		str_parse += output[i];
		str_parse += "\n";
	}

	json_registers = Parser.parse(str_parse);
}

void Debugger_GDB::ParseDissasembly() {

	std::wstring command = L"disassemble 0x" + to_hex_wstring(section_text_begin_adress) + L", 0x" + to_hex_wstring(section_text_end_adress) + L"\n";
	std::vector<std::string> output = SendCommandAndWaitAnswerByKeyWord(command, "(gdb) ");


	std::string str_parse;
	for (int i = 0; i < output.size(); i++) {
		str_parse += output[i];
		str_parse += "\n";
	}

	json_dissasembly = Parser.parse(str_parse);
}

void Debugger_GDB::ParseRangesTextSection() {
	std::vector<std::string> output = SendCommandAndWaitAnswerByKeyWord(L"maintenance info sections\n", "(gdb) ");

	output.pop_back();
	output.pop_back();
	output.erase(output.begin());
	output.erase(output.begin());


	for (size_t i = 0; i < output.size(); i++) {

		auto pos_begin_value = output[i].find("0x");
		auto pos_end_value = output[i].find_first_of(" ", pos_begin_value);

		std::string value_candidate = output[i].substr(pos_begin_value, pos_end_value - pos_begin_value);

		auto delimeter = value_candidate.find_first_of("-");

		std::string begin_value = value_candidate.substr(0, delimeter);
		std::string end_value = value_candidate.substr(delimeter + 2);

		begin_value.erase(begin_value.begin(), begin_value.begin() + 2);
		end_value.erase(end_value.begin(), end_value.begin() + 2);


		auto pos_begin_section = output[i].find(".");
		auto pos_end_section = output[i].find_first_of(" ", pos_begin_section);

		std::string section_candidate = output[i].substr(pos_begin_section, pos_end_section - pos_begin_section);


		if (section_candidate == ".text") {
			SetAdressToSectionText(
				strtoull(begin_value.c_str(), NULL, 16),
				strtoull(end_value.c_str(), NULL, 16)
			);
			break;
		}

	}

	//std::cout << std::hex << section_text_begin_adress << " " << section_text_end_adress << std::endl;

}

bool Debugger_GDB::IsRunning() {
	return DebuggerRunner->IsRunning();
}

void Debugger_GDB::SetBreakpointFromCode(const std::string file_name,const std::string& code_line) {


	std::string str_addr;

	for (auto& [key, value] : json_dissasembly["addr"].items()) {

		if (value.contains("File")) {

			bool equalName = value["File"]["Name"].get<std::string>() == file_name;
			bool equalLine = value["File"]["Line"].get<std::string>() == code_line;

			if (equalName && equalLine) {
				str_addr = key;
				break;
			}
		}

	}


}


void Debugger_GDB::MakeJSON_CodeWithAdresses() {
	for (auto& [key, value] : json_dissasembly["addr"].items()) {

		const uint64_t valueOffset = value["offset"].get<uint64_t>();

		for (int i = 0; i < ListingFileInfo.size(); i++) {


			const bool haveOffset = ListingFileInfo[i].offset_byte != UINT64_MAX;
			const bool correctSection = ListingFileInfo[i].wrongSection == false && ListingFileInfo[i].isSectionText;
			const bool equalsOffsets = valueOffset == ListingFileInfo[i].offset_byte;

			if (haveOffset && correctSection && equalsOffsets) {
				value["File"]["Name"] = ListingFileInfo[i].file_name;
				value["File"]["Line"] = ListingFileInfo[i].str_line;
				break;
			}

		}

	}

	//std::cout << json_dissasembly.dump(4) << std::endl;
}


void Debugger_GDB::StepInstruction() {

	DebuggerRunner->SetInput(L"-exec-next-instruction\n");
	auto answer = WaitAnswerByFindKeywordOnLastLine("*stopped");

	std::string str_adress;
	if (answer.back().find("*stopped") != std::string::npos) {
		auto pos_begin = answer.back().find("addr=\"");
		
		if (pos_begin != std::string::npos) {

			auto pos_end = answer.back().find("\"", pos_begin + 7);

			str_adress = answer.back().substr(pos_begin, pos_end - pos_begin);
			str_adress.erase(str_adress.begin(), str_adress.begin() + 2 + 6);

			currentAdress = strtoull(str_adress.c_str(), NULL, 16);
		}

	}

	uint64_t Value_check = currentAdress - section_text_begin_adress;
	for (int i = 0; i < ListingFileInfo.size(); i++) {
		if (ListingFileInfo[i].offset_byte == Value_check) {
			json_dissasembly["current"] = str_adress;
		}
	}


	ParseRegisters();
}

void Debugger_GDB::StepLineCode() {
	DebuggerRunner->SetInput(L"-exec-step\n ");
}

void Debugger_GDB::Run_Continously() {
	DebuggerRunner->SetInput(L"-exec-run\n ");
}

void Debugger_GDB::Run_StepByStep() {
	DebuggerRunner->SetInput(L"starti\n ");
}

void Debugger_GDB::Pause() {
	DebuggerRunner->SetInput(L"-exec-interrupt\n ");

	SetAllBreakpoints();

	//DeleateAllBreakpoints();
}

void Debugger_GDB::Continue() {
	DebuggerRunner->SetInput(L"-exec-continue\n ");
}

void Debugger_GDB::Exit() {
	DebuggerRunner->SetInput(L"exit\n ");
}



void Debugger_GDB::GetVersionFromExecutable() {
	DebuggerRunner->Run(L"");
	while (DebuggerRunner->IsRunning()) {
		if (DebuggerRunner->IsOutputChanged()) {
			Version = DebuggerRunner->GetOutput().front();
			Version.pop_back();
			DebuggerRunner->Terminate();
		}
	}
}



std::vector<std::string> Debugger_GDB::GetOutput() {
	return DebuggerRunner->GetOutput();
}
std::string Debugger_GDB::GetVersion() {
	return Version;
}


Debugger_GDB::Debugger_GDB(const std::wstring& path2executable) {
	DebuggerRunner = new ProcessRuner(path2executable);
	GetVersionFromExecutable();

}

Debugger_GDB::~Debugger_GDB() {

}
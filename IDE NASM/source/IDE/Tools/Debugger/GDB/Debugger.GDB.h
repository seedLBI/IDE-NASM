#ifndef DEBUGGER_GDB_H
#define DEBUGGER_GDB_H

#include "Utils/ProcessRuner/ProcessRuner.h"
#include "IDE/Tools/Compiler/NASM/Parser_ListingFile/Parser.ListingFile.Nasm.h"
#include "IDE/Tools/Debugger/Debugger.h"
#include "Parser/Parser.GDB.h"

#include "Utils/Text/Utils.Text.h"

#include <sstream>

class Debugger_GDB : public Debugger
{
public:
	Debugger_GDB(const std::wstring& path2GDB);
	~Debugger_GDB();


	void SetListingInfo(const std::vector<InfoLineListing>& ListingFileInfo);
	void SetEntryPoint(const std::wstring& NameEntryPoint);


	void Debug(const std::wstring& path2debug) override;
	std::vector<std::string> GetOutput() override;
	std::string GetVersion() override;
	bool IsRunning();

	void Run_Continously();
	void Run_StepByStep();

	nlohmann::json GetRegisters();

	void SetBreakpointFromCode(const std::string file_name, const std::string& code_line);



	void Pause();
	void Continue();
	void StepInstruction();
	void StepLineCode();
	void Exit();


private:
	ProcessRuner* DebuggerRunner = nullptr;

	ParserGDB Parser;

	std::string Version;
	void GetVersionFromExecutable();

	std::vector<InfoLineListing> ListingFileInfo;
	std::wstring NameEntryPoint;



	void SetAllBreakpoints();
	void DeleateAllBreakpoints();


	nlohmann::json GetFirstParse();


	void ParseRangesTextSection();
	void ParseDissasembly();
	void ParseRegisters();

	void MakeJSON_CodeWithAdresses();

	nlohmann::json json_codeAdressed;


	nlohmann::json json_dissasembly;
	nlohmann::json json_registers;

	void SendCommand(const std::wstring& command);
	std::vector<std::string> WaitAnswerByKeyWord(const std::string& keyword);
	std::vector<std::string> WaitAnswerByFindKeywordOnLastLine(const std::string& keyword);

	std::vector<std::string> SendCommandAndWaitAnswerByKeyWord(const std::wstring& command, const std::string& keyword);


	void WaitForExecAsync_Stopped();


	void ClearData();

	void SetAdressToSectionText(const uint64_t& begin, const uint64_t& end);

	uint64_t currentAdress = 0;

	std::vector<uint64_t> offsets_bytes;

	uint64_t section_text_begin_adress = 0;
	uint64_t section_text_end_adress = 0;
};


#endif
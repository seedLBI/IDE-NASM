#ifndef BUILD_MANAGER_H
#define BUILD_MANAGER_H


#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <any>

#include "IDE/Core/Solution/Solution.h"

#include "IDE/Tools/Compiler/NASM/Parser_ListingFile/Parser.ListingFile.Nasm.h"
#include "IDE/Tools/Compiler/NASM/Parser_OutputNasm/NASM.Parser.Output.h"

#include "IDE/Utils/InfoLines/IDE/InfoLine.IDE.Creater.h"


#include "IDE/Tools/Compiler/NASM/Compiler.NASM.h"
#include "IDE/Tools/Linker/GCC/Linker.GCC.h"
#include "IDE/Tools/Debugger/GDB/Debugger.GDB.h"
#include "IDE/Tools/DebugConsole/DebugConsole.h"

#include "Utils/File/Utils.File.h"
#include "Utils/Synchronization/Mutex/Mutex.h"
#include "Utils/Synchronization/Event/Event.h"

#include "IDE/GUI/OutlineStatus/OutlineStatus.h"


#include "Utils/Synchronization/FileMapping/FileMapping.h"
#include "Utils/Synchronization/Mutex/Mutex.h"






enum class BuildManager_DefaultPlatforms {
	Release,
	Debug,
	Custom
};

struct SettingBuildTemplate {
	BuildManager_DefaultPlatforms platform;

	std::string str_NameTemplate;

	bool flag_Debug = false;

	// Compiler
	Compiler_ARGS_ARCHITECTURE		arg_architecture;
	Compiler_ARGS_OPTIMIZATION		arg_optimisation;
	Compiler_ARGS_ERRORS_WARNINGS	arg_errors_preset;

	std::string str_define;
	std::string str_entryPoint;

};

enum BuildManager_States {
	BuildManager_Compiling,
	BuildManager_Linking,
	BuildManager_Running,
	BuildManager_Debuging,
	BuildManager_Debuging_StepInto,
	BuildManager_Debuging_StepOver,
	BuildManager_Debuging_Start,
	BuildManager_Debuging_Pause,
	BuildManager_Free
};

class BuildManager {
private:
	bool flag_IsBusy = false;
	Event event_WaitingNewWork;
	BuildManager_States CurrentState = BuildManager_Free;
public:
	BuildManager(Solution* solution, OutlineStatus* outlineStatus);
	~BuildManager();

	void Compile();
	void CompileAndLink();
	void Run();
	void TerminateRun();

	void CancelBuild();
	void ClearOutput();

	bool IsBusy();
	
	std::vector <std::string> GetOutputAll_Unformatted();
	std::vector <std::any> GetOutputAll_Formatted();



	void SetArch(const Compiler_ARGS_ARCHITECTURE& arch);
	void SetPlatform(const BuildManager_DefaultPlatforms& platform);
	void SetEntryPoint(const std::string& entryPoint);
	//void LoadBreakpoints(std::vector<std::string>)

	// Debug Control
	void Debug_StepInto(); // шаг с заходом
	void Debug_StepOver(); // шаг с обходом
	void Debug_Start();
	void Debug_StartWithPause();
	void Debug_Pause();
	void Debug_Terminate();

	bool GetFlag_AvailableNextStep();


	BuildManager_States GetState();
	SettingBuildTemplate& GetCurrentSettings();


private:
	Solution* solution = nullptr;
	OutlineStatus* outlineStatus = nullptr;

	Compiler_Nasm*	compiler	 = nullptr;
	Linker_GCC*		linker		 = nullptr;
	Debugger_GDB*	debugger	 = nullptr;
	DebugConsole*	debugConsole = nullptr;

	// NASM
	ParserListingFile* NASM_parserListingFile = nullptr;
	ParserOutputNASM* NASM_parserOutput = nullptr;

	//IDE
	InfoLineIDE_Creater* IDE_InfoCreater = nullptr;

	const std::wstring path_Compiler	 = L"tools\\nasm\\nasm.exe";
	const std::wstring path_Linker_64	 = L"tools\\linker\\MinGW64\\bin\\gcc.exe";
	const std::wstring path_Linker_32	 = L"tools\\linker\\MinGW\\bin\\gcc.exe";
	const std::wstring path_Debugger	 = L"tools\\debugger\\bin\\gdb.exe";
	const std::wstring path_DebugConsole = L"tools\\debug_console\\DebugProcessRunner.exe";

	std::vector<std::string> OutputAll_Unformatted;
	std::vector<std::any> OutputAll_Formatted;

	std::thread mainThread;

	SettingBuildTemplate template_Debug;
	SettingBuildTemplate template_Release;

	SettingBuildTemplate template_Current;

	void InitDefaultTemplates();

	void CheckExistTools();


	std::wstring GetCurrentPathOutput();

	void MakeWork_Compile();
	void MakeWork_Link();
	void MakeWork_Run();
	void MakeWork_Debug();



	const std::string WORK_TYPE_COMPILE		= "Compile";
	const std::string WORK_TYPE_LINK		= "Link";
	const std::string WORK_TYPE_RUN			= "Run";
	const std::string WORK_TYPE_DEBUG		= "Debug";



	std::string CurrentWork = "";

	std::vector<std::string> Works;

	void ThreadLoop();
};




#endif
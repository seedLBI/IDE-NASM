#include "BuildManager.h"

BuildManager::BuildManager(Solution* solution, OutlineStatus* outlineStatus) {
#ifdef _DEBUG
	std::cout << "BuildManager::BuildManager()\n";
#endif // _DEBUG

	this->solution = solution;
	this->outlineStatus = outlineStatus;

	CheckExistTools();
	compiler	= new Compiler_Nasm(path_Compiler);
	linker		= new Linker_GCC(path_Linker_32, path_Linker_64);
	debugger	= new Debugger_GDB(path_Debugger);
	debugConsole = new DebugConsole(path_DebugConsole);

#ifdef _DEBUG
	while (compiler->IsRunning()) { }
	while (linker->IsRunning()) { }
	while (debugger->IsRunning()) { }

	std::cout << compiler->GetVersion() << std::endl;
	std::cout << linker->GetVersion() << std::endl;
	std::cout << debugger->GetVersion() << std::endl;
#endif // _DEBUG

	InitDefaultTemplates();


	NASM_parserListingFile = new ParserListingFile;
	NASM_parserOutput = new ParserOutputNASM;
	IDE_InfoCreater = new InfoLineIDE_Creater;

	mainThread = std::thread(&BuildManager::ThreadLoop,this);

	event_WaitingNewWork.Reset();

	template_Current = template_Debug;
}

BuildManager::~BuildManager() {
#ifdef _DEBUG
	std::cout << "BuildManager::~BuildManager()\n";
#endif // _DEBUG
	mainThread.join();
}


std::vector < std::string > BuildManager::GetOutputAll_Unformatted(){
	return OutputAll_Unformatted;
}
std::vector <std::any> BuildManager::GetOutputAll_Formatted() {
	return OutputAll_Formatted;
}

std::wstring BuildManager::GetCurrentPathOutput() {

	auto preparedName = stringUTF8_to_wstring(transliterate_rus(wstring_to_stringUTF8(solution->GetInfo().NameFileSolution)));

	if (template_Current.flag_Debug) {
		if (template_Current.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows32)
			return solution->GetInfo().GetPathAbsolute() + L"\\x86\\debug\\" + preparedName;
		else if (template_Current.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows64)
			return solution->GetInfo().GetPathAbsolute() + L"\\x64\\debug\\" + preparedName;
	}
	else {
		if (template_Current.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows32)
			return solution->GetInfo().GetPathAbsolute() + L"\\x86\\release\\" + preparedName;
		else if (template_Current.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows64)
			return solution->GetInfo().GetPathAbsolute() + L"\\x64\\release\\" + preparedName;
	}

	return {};
}




void BuildManager::ThreadLoop() {
	while (1) {
		event_WaitingNewWork.Wait();


		for (size_t i = 0; i < Works.size(); i++) {

			if (Works[i]      == WORK_TYPE_COMPILE) {

				CurrentState = BuildManager_Compiling;


				OutputAll_Formatted.push_back("nasm.exe " + wstring_to_stringUTF8(compiler->CreateArgumentsLine()));
				OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_BeginCompile());

				outlineStatus->SetStatus(Type_OutlineStatus::OutlineStatus_Building);

				compiler->Compile(stringUTF8_to_wstring(solution->GetInfo().mainFile));

				while (compiler->IsRunning()) {
					Sleep(100);
				}

				auto outputData = compiler->GetOutput();

				if (outputData.empty() == false) {
					if (outputData.back().empty()) {
						outputData.pop_back();
					}
				}

#ifdef _DEBUG
				for (int i = 0; i < outputData.size(); i++) {
					std::cout << outputData[i] << std::endl;
				}
#endif // _DEBUG
				
				NASM_parserOutput->Parse(outputData);
				auto OutputCompiler = NASM_parserOutput->GetOutput();


				for (size_t i = 0; i < OutputCompiler.size(); i++)
					OutputAll_Formatted.push_back(OutputCompiler[i]);


				const bool HaveErrors = NASM_parserOutput->HaveError();
				const int countErrors = NASM_parserOutput->GetCountErrors();

				bool HaveWrongDataSection = false;


				if (NASM_parserOutput->HaveError() == false) {

					std::wstring PathToListingFile = GetCurrentPathOutput() + L"\\" + solution->GetInfo().GetTransliterateNameProject() + L".lst";

					if (isFileExist(PathToListingFile)) {
						NASM_parserListingFile->Parse(PathToListingFile, stringUTF8_to_wstring(solution->GetInfo().mainFile));

						auto dataListingFile = NASM_parserListingFile->GetOutput();

						HaveWrongDataSection = NASM_parserListingFile->HaveWarnings();


#ifdef _DEBUG
						for (int i = 0; i < dataListingFile.size(); i++) {
							std::cout << dataListingFile[i].offset_byte << ": " << dataListingFile[i].file_name << ": " << dataListingFile[i].number_line_file << ": " << dataListingFile[i].str_line << std::endl;
						}
#endif // _DEBUG
					}



				}


				OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndCompile());


				if (HaveWrongDataSection)
					OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndCompile_Bad_Data_Wrong_Section());

				if (HaveErrors)
					OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndCompile_Bad_Error(countErrors));
				else
					OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndCompile_Good());

				if (HaveErrors) {
					break;
				}
			}
			else if (Works[i] == WORK_TYPE_LINK) {

				CurrentState = BuildManager_Linking;


				
				std::string args_gcc = wstring_to_stringUTF8(linker->MakeArgumentLine());
				ReplaceAll(args_gcc, wstring_to_stringUTF8(solution->GetInfo().GetPathAbsolute()) + "\\", "");
				OutputAll_Formatted.push_back("gcc.exe " + args_gcc);


				OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_BeginLink());

				outlineStatus->SetStatus(Type_OutlineStatus::OutlineStatus_Building);

				linker->Link();

				while (linker->IsRunning()) {
					Sleep(100);
				}

				auto outputData = linker->GetOutput();

#ifdef _DEBUG
				for (int i = 0; i < outputData.size(); i++) {
					std::cout << outputData[i] << std::endl;
				}
#endif // _DEBUG

				OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndLink());


				bool AllGood = false;

				AllGood = isFileExist(GetCurrentPathOutput() + L".exe");


				if (AllGood)
					OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndLink_Good());
				else {
					OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndLink_Bad_Error());
					Works.clear();
				}

			}
			else if (Works[i] == WORK_TYPE_RUN) {
				CurrentState = BuildManager_Running;

				std::string runner_args = wstring_to_stringUTF8(GetCurrentPathOutput() + L".exe");
				ReplaceAll(runner_args, wstring_to_stringUTF8(solution->GetInfo().GetPathAbsolute()) + "\\", "");
				OutputAll_Formatted.push_back("runner.exe \"" + runner_args + "\"");

				OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_BeginRun());

				outlineStatus->SetStatus(Type_OutlineStatus::OutlineStatus_Running);


				debugConsole->Run();

				while (debugConsole->isRunning()) {
					Sleep(30);
				}


				OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndRun_Good());


				if (debugConsole->isTerminated())
				{
					OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndRun_Terminated());
				}
				if (debugConsole->GoodEnding()) {
					OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndRun_Info(debugConsole->getExitCode()));

				}

			}

		}

		
		Works.clear();

		outlineStatus->SetStatus(Type_OutlineStatus::OutlineStatus_Nothing);

		CurrentState = BuildManager_Free;



		flag_IsBusy = false;

		event_WaitingNewWork.Reset();
	}
}


void BuildManager::InitDefaultTemplates() {
	template_Debug.flag_Debug        = true;
	template_Debug.arg_optimisation  = Compiler_ARGS_OPTIMIZATION::NoOptimization;
	template_Debug.arg_errors_preset = Compiler_ARGS_ERRORS_WARNINGS::EnableAll;
	template_Debug.arg_architecture  = Compiler_ARGS_ARCHITECTURE::Windows64;
	template_Debug.str_entryPoint	 = "main";
	template_Debug.str_define		 = "DEBUG";
	template_Debug.str_NameTemplate  = "Debug";
	template_Debug.platform			 = BuildManager_DefaultPlatforms::Debug;

	
	template_Release.flag_Debug			= false;
	template_Release.arg_optimisation	= Compiler_ARGS_OPTIMIZATION::MultipassOptimization;
	template_Release.arg_errors_preset	= Compiler_ARGS_ERRORS_WARNINGS::EnableAll;
	template_Release.arg_architecture	= Compiler_ARGS_ARCHITECTURE::Windows64;
	template_Release.str_entryPoint		= "main";
	template_Release.str_define			= "RELEASE";
	template_Release.str_NameTemplate	= "Release";
	template_Release.platform			= BuildManager_DefaultPlatforms::Release;
}

SettingBuildTemplate& BuildManager::GetCurrentSettings() {
	return template_Current;
}
BuildManager_States BuildManager::GetState() {
	return CurrentState;
}


void BuildManager::SetArch(const Compiler_ARGS_ARCHITECTURE& arch) {

	if (arch == Compiler_ARGS_ARCHITECTURE::Windows32) {
		template_Release.arg_architecture = Compiler_ARGS_ARCHITECTURE::Windows32;
		template_Debug.arg_architecture   = Compiler_ARGS_ARCHITECTURE::Windows32;
		template_Current.arg_architecture = Compiler_ARGS_ARCHITECTURE::Windows32;
	}
	else if (arch == Compiler_ARGS_ARCHITECTURE::Windows64) {
		template_Release.arg_architecture = Compiler_ARGS_ARCHITECTURE::Windows64;
		template_Debug.arg_architecture   = Compiler_ARGS_ARCHITECTURE::Windows64;
		template_Current.arg_architecture = Compiler_ARGS_ARCHITECTURE::Windows64;
	}

}
void BuildManager::SetPlatform(const BuildManager_DefaultPlatforms& platform) {
	if (platform == BuildManager_DefaultPlatforms::Release)
		template_Current = template_Release;
	else if (platform == BuildManager_DefaultPlatforms::Debug)
		template_Current = template_Debug;
}
void BuildManager::SetEntryPoint(const std::string& entryPoint) {
	template_Release.str_entryPoint = entryPoint;
	template_Debug.str_entryPoint = entryPoint;
	template_Current.str_entryPoint = entryPoint;
}

void BuildManager::CheckExistTools() {
	if (isFileExist(path_Compiler) == false) {
		MessageBoxW(0, L"А где компилятор NASM?", L"Ошибка нахождения инструментов", MB_ICONERROR | MB_OK);
		exit(404);
	}
	if (isFileExist(path_Linker_32) == false) {
		MessageBoxW(0, L"А где 32-x битный сборщик GCC?", L"Ошибка нахождения инструментов", MB_ICONERROR | MB_OK);
		exit(404);
	}
	if (isFileExist(path_Linker_64) == false) {
		MessageBoxW(0, L"А где 64-x битный сборщик GCC?", L"Ошибка нахождения инструментов", MB_ICONERROR | MB_OK);
		exit(404);
	}
	if (isFileExist(path_Debugger) == false) {
		MessageBoxW(0, L"А где отладчик GDB?", L"Ошибка нахождения инструментов", MB_ICONERROR | MB_OK);
		exit(404);
	}
	if (isFileExist(path_DebugConsole) == false) {
		MessageBoxW(0, L"А где консольный отладчик?", L"Ошибка нахождения инструментов", MB_ICONERROR | MB_OK);
		exit(404);
	}
}

void BuildManager::ClearOutput() {
	OutputAll_Unformatted.clear();
	OutputAll_Formatted.clear();
}

void BuildManager::Compile() {
#ifdef _DEBUG
	std::cout << "void BuildManager::Compile()\n";
#endif // _DEBUG

	if (flag_IsBusy == false) {

		ClearOutput();

		MakeWork_Compile();

		flag_IsBusy = true;
		event_WaitingNewWork.Set();
	}

}

void BuildManager::CompileAndLink() {
#ifdef _DEBUG
	std::cout << "void BuildManager::CompileAndLink()\n";
#endif // _DEBUG

	if (flag_IsBusy == false) {

		ClearOutput();

		MakeWork_Compile();
		MakeWork_Link();


		flag_IsBusy = true;
		event_WaitingNewWork.Set();
	}

}

void BuildManager::Run() {
	if (flag_IsBusy == false) {

		ClearOutput();

		MakeWork_Run();

		flag_IsBusy = true;
		event_WaitingNewWork.Set();
	}
}

void BuildManager::TerminateRun() {
	debugConsole->Terminate();
}


void BuildManager::CancelBuild() {
	Works.clear();
	
}





void BuildManager::Debug_StepInto() {

}
void BuildManager::Debug_StepOver() {

}

void BuildManager::Debug_Start() {
	if (flag_IsBusy == false) {

		MakeWork_Debug();

	}
}
void BuildManager::Debug_StartWithPause() {
	if (flag_IsBusy == false) {
		MakeWork_Debug();
	}
}

void BuildManager::Debug_Pause() {

}
void BuildManager::Debug_Terminate() {

}

bool BuildManager::GetFlag_AvailableNextStep() {
	return false;
}





void BuildManager::MakeWork_Compile() {



	if (compiler->IsRunning()) {
		return;
	}

	solution->SaveAllFiles();

	compiler->EnableOutputFile();

	compiler->Set_Arg_Architecture(template_Current.arg_architecture);
	compiler->Set_Arg_ErrorsWarning(template_Current.arg_errors_preset);
	compiler->Set_Arg_Optimization(template_Current.arg_optimisation);

	compiler->SetNameFile(stringUTF8_to_wstring(solution->GetInfo().name));

	auto preparedName = transliterate_rus(wstring_to_stringUTF8(solution->GetInfo().NameFileSolution));

	compiler->SetIncludePath(L"source");
	compiler->SetNameFile(stringUTF8_to_wstring(preparedName));
	compiler->SetDirectoryForRun(solution->GetInfo().GetPathAbsolute());

	if (template_Current.flag_Debug) {
		compiler->EnableDebug();
		if (template_Current.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows32)
			compiler->SetOutputPath(L"x86\\debug");
		else if (template_Current.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows64)
			compiler->SetOutputPath(L"x64\\debug");
		
	}
	else {
		compiler->DisableDebug();

		if (template_Current.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows32)
			compiler->SetOutputPath(L"x86\\release");
		else if (template_Current.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows64)
			compiler->SetOutputPath(L"x64\\release");
	}


	if (HaveFolder(solution->GetInfo().GetPathAbsolute() + L"\\x86") == false) {
		CreateFolder(solution->GetInfo().GetPathAbsolute() + L"\\x86");
		CreateFolder(solution->GetInfo().GetPathAbsolute() + L"\\x86\\debug");
		CreateFolder(solution->GetInfo().GetPathAbsolute() + L"\\x86\\release");
	}

	if (HaveFolder(solution->GetInfo().GetPathAbsolute() + L"\\x64") == false) {
		CreateFolder(solution->GetInfo().GetPathAbsolute() + L"\\x64");
		CreateFolder(solution->GetInfo().GetPathAbsolute() + L"\\x64\\debug");
		CreateFolder(solution->GetInfo().GetPathAbsolute() + L"\\x64\\release");
	}

	if (HaveFolder(solution->GetInfo().GetPathAbsolute() + L"\\x86\\debug") == false)
		CreateFolder(solution->GetInfo().GetPathAbsolute() + L"\\x86\\debug");
	if (HaveFolder(solution->GetInfo().GetPathAbsolute() + L"\\x86\\release") == false)
		CreateFolder(solution->GetInfo().GetPathAbsolute() + L"\\x86\\release");


	if (HaveFolder(solution->GetInfo().GetPathAbsolute() + L"\\x64\\debug") == false)
		CreateFolder(solution->GetInfo().GetPathAbsolute() + L"\\x64\\debug");
	if (HaveFolder(solution->GetInfo().GetPathAbsolute() + L"\\x64\\release") == false)
		CreateFolder(solution->GetInfo().GetPathAbsolute() + L"\\x64\\release");

	Works.push_back(WORK_TYPE_COMPILE);
}

void BuildManager::MakeWork_Link() {

	if (linker->IsRunning())
		return;

	linker->SetEntryPoint(stringUTF8_to_wstring(template_Current.str_entryPoint));

	if (template_Current.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows32)
		linker->SetBitArch(LinkerArchs::Windows32);
	else if (template_Current.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows64)
		linker->SetBitArch(LinkerArchs::Windows64);


	if (template_Current.flag_Debug)
		linker->EnableDebug();
	else
		linker->DisableDebug();

	linker->SetPath_ObjectFile(GetCurrentPathOutput() + L".o");
	linker->SetPath_OutputExecutableFile(GetCurrentPathOutput() + L".exe");

	Works.push_back(WORK_TYPE_LINK);
}

void BuildManager::MakeWork_Run() {

	bool allOK = false;
	
	allOK = isFileExist(GetCurrentPathOutput() + L".exe");
	debugConsole->SetArgs(GetCurrentPathOutput() + L".exe");

	if (allOK == false) {
		OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndRun_No_Exe());
		MakeWork_Compile();
		MakeWork_Link();
	}
	else {
		MakeWork_Compile();
		MakeWork_Link();
	}


	Works.push_back(WORK_TYPE_RUN);
}

void BuildManager::MakeWork_Debug() {
	bool allOK = false;

	allOK = isFileExist(GetCurrentPathOutput() + L".exe");

	if (allOK == false) {
		OutputAll_Formatted.push_back(IDE_InfoCreater->MakeInfo_EndDebug_No_Exe());
		MakeWork_Compile();
		MakeWork_Link();
	}
	else {
		MakeWork_Compile();
		MakeWork_Link();
	}

}


bool BuildManager::IsBusy() {
	return flag_IsBusy;
}



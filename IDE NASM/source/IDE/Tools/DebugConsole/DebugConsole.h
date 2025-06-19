#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include <Windows.h>
#include <TlHelp32.h>
#include <string>

#include "Utils/Timer/Utils.Timer.h"
#include "Utils/File/Utils.File.h"

#include "Utils/Synchronization/Event/Event.h"
#include "Utils/Synchronization/Mutex/Mutex.h"
#include "Utils/Synchronization/FileMapping/FileMapping.h"

struct DataDebugProcess {
	bool isRunning = false;
	bool goodEnding = false;
	bool NeedTerminate = false;
	DWORD exitCode = -1;
};


class DebugConsole {
public:
	DebugConsole(const std::wstring& path);
	~DebugConsole();

	void SetArgs(const std::wstring& args);

	void Run();
	bool isRunning();

	void Terminate();
	bool isTerminated();

	bool GoodEnding();
	DWORD getExitCode();
private:
	std::wstring str_Path;
	std::wstring str_Args; 
	
	const std::wstring str_Title = L"Отладочная консоль IDE NASM";
	

	FileMapping <DataDebugProcess>* mapping_data = nullptr;
	Mutex* mutex_mapping_data = nullptr;
	Event* event_available_run = nullptr; 

	PROCESS_INFORMATION pi;
	STARTUPINFOW si;

	bool flag_IsRunning = false;

	DataDebugProcess tempData;


	Timer timer;

	void Close();
	void KillExistingProcesses(const std::wstring& exePath);
};



#endif
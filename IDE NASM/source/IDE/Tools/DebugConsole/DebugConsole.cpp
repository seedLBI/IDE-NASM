#include "DebugConsole.h"


DebugConsole::DebugConsole(const std::wstring& path) {
	str_Path = path;

	mutex_mapping_data = new Mutex("MUTEX_FILE_MAPPING_DEBUG_CONSOLE");
	mapping_data = new FileMapping<DataDebugProcess>(L"tools\\debug_console\\FileMapping.data", true);
	event_available_run = new Event(EVENT_TYPE_RESET::MANUAL_RESET, false, "EVENT_ABAILABLE_RUN_DEBUG_CONSOLE");


}

DebugConsole::~DebugConsole() {

}
void DebugConsole::SetArgs(const std::wstring& args) {
	str_Args = args;
}


void DebugConsole::Run() {

	KillExistingProcesses(str_Path);

	event_available_run->Reset();

	tempData.exitCode = -1;
	tempData.goodEnding = false;
	tempData.isRunning = false;

	pi = { 0 };
	si = { sizeof(si) };
	//si.dwFlags = STARTF_USESTDHANDLES;
	//si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	//si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	//si.hStdError = GetStdHandle(STD_ERROR_HANDLE);


	mutex_mapping_data->Enter();
		mapping_data->get()->NeedTerminate = false;
		mapping_data->get()->isRunning = false;
		mapping_data->get()->goodEnding = false;
		mapping_data->get()->exitCode = -1;
	mutex_mapping_data->Leave();

	std::wstring CompletePath = L"\"" + str_Path + L"\" " + L"\"" + str_Args + L"\"";

	std::wcout << L"[" + CompletePath + L"]\n";

	bool Created = CreateProcessW(NULL, (LPWSTR)CompletePath.c_str(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (Created == false) {
		std::cout << "ERROR\n";
		return;
	}

	const double available_time = 2.f;
	timer.start();
	while (1) {
		timer.stop();

		std::cout << "TRYING CREATE PROCESS\n";

		bool checkProcess_Run = false;

		mutex_mapping_data->Enter();
			checkProcess_Run = mapping_data->get()->isRunning;
		mutex_mapping_data->Leave();

		if (checkProcess_Run == true) {
			event_available_run->Set();
			break;
		}
		std::cout << timer.elapsedSeconds() << "  " << available_time << std::endl;
		if (timer.elapsedSeconds() >= available_time) {
			Close();
			return;
		}


		Sleep(30);
	}

	std::cout << "PROCESS CREATED\n";


	flag_IsRunning = true;
}


bool DebugConsole::isRunning() {

	if (flag_IsRunning == false) return false;


	DWORD exitCode;



	
	if (GetExitCodeProcess(pi.hProcess, &exitCode)) {

		if (exitCode == STILL_ACTIVE) {

			mutex_mapping_data->Enter();
				tempData = *mapping_data->get();
			mutex_mapping_data->Leave();


			if (tempData.isRunning)
				return true;
			else
				return false;
		}
		else {
			WaitForSingleObject(pi.hThread, INFINITE);
			Close();
			return false;
		}
	}
	else {
		std::cout << "Error GetExitCodeProcess :[" << GetLastError() << "]" << std::endl;
		Close();
		return false;
	}
	

	return false;
}

void DebugConsole::Terminate() {
	mutex_mapping_data->Enter();
		mapping_data->get()->NeedTerminate = true;
	mutex_mapping_data->Leave();
}

bool DebugConsole::isTerminated() {
	bool result;

	mutex_mapping_data->Enter();
		result = mapping_data->get()->NeedTerminate;
	mutex_mapping_data->Leave();

	return result;
}

void DebugConsole::Close() {
	if (pi.hProcess != NULL) {
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
	}
	if (pi.hThread != NULL) {
		CloseHandle(pi.hThread);
		pi.hThread = NULL;
	}

	flag_IsRunning = false;
}

bool DebugConsole::GoodEnding() {

	mutex_mapping_data->Enter();
		bool result = mapping_data->get()->goodEnding;
	mutex_mapping_data->Leave();

	return result;
}
DWORD DebugConsole::getExitCode() {
	mutex_mapping_data->Enter();
		DWORD result = mapping_data->get()->exitCode;
	mutex_mapping_data->Leave();

	return result;
}

void DebugConsole::KillExistingProcesses(const std::wstring& exePath) {
	// Получаем идентификатор текущего процесса, чтобы не убить себя
	DWORD currentPID = GetCurrentProcessId();

	// Снимок всех процессов
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
		return;

	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(pe);

	const std::wstring fullPath = (GetPathDirectory() + L"\\" + str_Path);

	if (Process32FirstW(hSnap, &pe)) {
		do {
			if (pe.th32ProcessID == currentPID)
				continue;

			// Открываем процесс, чтобы узнать путь к его модулю
			HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
			if (!hProc)
				continue;

			// Читаем полный путь к экзешнику
			wchar_t pathBuf[MAX_PATH];
			DWORD sz = _countof(pathBuf);
			if (QueryFullProcessImageNameW(hProc, 0, pathBuf, &sz)) {
				//std::wcout << L"[" << pathBuf << L"] [" + fullPath + L"]\n";
				if (_wcsicmp(pathBuf, fullPath.c_str()) == 0) {
					// Совпало — убиваем
					TerminateProcess(hProc, 1);
				}
			}
			CloseHandle(hProc);
		} while (Process32NextW(hSnap, &pe));
	}

	CloseHandle(hSnap);
}
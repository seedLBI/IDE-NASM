#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <locale>

#include "Utils/Synchronization/FileMapping/FileMapping.h"
#include "Utils/Synchronization/Mutex/Mutex.h"
#include "Utils/Synchronization/Event/Event.h"


struct DataDebugProcess {
    bool isRunning = false;
    bool goodEnding = false;
    bool NeedTerminate = false;
    DWORD exitCode = -1;
};

FileMapping <DataDebugProcess>* mapping_data = nullptr;



int wmain(int argc, wchar_t* argv[]) {

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    try {
        std::locale::global(std::locale("ru_RU.UTF-8"));
    }
    catch (...) {
        std::locale::global(std::locale("Russian_Russia.65001"));
    }


    if (argc < 2) {
        return -1;
    }

    std::wstring exe = argv[1];

#ifdef _DEBUG
    for (int i = 0; i < argc; i++) {
        std::wcout << L"[" << argv[i] << L"]" << std::endl;
    }
#endif // _DEBUG

    Mutex mutex_mapping_data("MUTEX_FILE_MAPPING_DEBUG_CONSOLE");
    Event event_available_run(EVENT_TYPE_RESET::MANUAL_RESET, false, "EVENT_ABAILABLE_RUN_DEBUG_CONSOLE");
    mapping_data = new FileMapping <DataDebugProcess>(L"FileMapping.data", true);


    mutex_mapping_data.Enter();
        mapping_data->get()->isRunning = true;
    mutex_mapping_data.Leave();

    // Ждём когда нам разрешат...
    event_available_run.Wait();

    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFOW si = { sizeof(si) };
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

#ifdef _DEBUG
    std::wcout << L"Creation process: " << exe << std::endl;
#endif // _DEBUG


    if (CreateProcessW(
        exe.c_str(),    // Исполняемый модуль
        NULL,           // Командная строка
        NULL,           // Безопасность процесса
        NULL,           // Безопасность потока
        TRUE,          // Не наследовать дескрипторы
        NULL, // Флаги создания
        NULL,           // Окружение
        NULL,           // Текущий каталог
        &si,
        &pi))
    {
#ifdef _DEBUG
        std::wcout << L"Created process: " << exe << std::endl;
#endif // _DEBUG

        SetConsoleTitleW(L"Отладочная консоль IDE NASM");

        bool NeedTerminate = false;


        DWORD exitCode;

        while (1) {

            mutex_mapping_data.Enter();
                NeedTerminate = mapping_data->get()->NeedTerminate;
            mutex_mapping_data.Leave();

            if (NeedTerminate) {
#ifdef _DEBUG
                std::wcout << L"Принудительно остановили " << std::endl;
#endif // _DEBUG
                break;
            }

            if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
                if (exitCode != STILL_ACTIVE) {
                    break;
                }
            }


            Sleep(30);
        }


        if (NeedTerminate) {
            if (pi.hProcess != NULL) {
                TerminateProcess(pi.hProcess, 0);
                CloseHandle(pi.hProcess);
                pi.hProcess = NULL;
            }
            if (pi.hThread != NULL) {
                CloseHandle(pi.hThread);
                pi.hThread = NULL;
            }

            mutex_mapping_data.Enter();
                mapping_data->get()->exitCode = -1;
                mapping_data->get()->goodEnding = false;
                mapping_data->get()->isRunning = false;
            mutex_mapping_data.Leave();


            wprintf(L"\n\n%s Процесс был принудительно завершён\n", exe.c_str());
            wprintf(L"Нажмите любую клавишу, чтобы закрыть это окно...");
        }
        else {

            mutex_mapping_data.Enter();
            GetExitCodeProcess(pi.hProcess, &mapping_data->get()->exitCode);
            mapping_data->get()->goodEnding = true;
            mapping_data->get()->isRunning = false;
            mutex_mapping_data.Leave();


            wprintf(L"\n\n[%s] Процесс завершил работу с кодом %d (0x%x)\n", exe.c_str(), mapping_data->get()->exitCode, mapping_data->get()->exitCode);
            wprintf(L"Нажмите любую клавишу, чтобы закрыть это окно...");


            _getch();

            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
        }
    }
    else
    {
#ifdef _DEBUG
        std::wcout << L"Error create process: " << exe << std::endl;
#endif // _DEBUG
        mutex_mapping_data.Enter();
            mapping_data->get()->goodEnding = false;
            mapping_data->get()->isRunning = false;
        mutex_mapping_data.Leave();

#ifdef _DEBUG
        std::wcout << L"mapping_data->get()->isRunning = false;" << std::endl;
#endif // _DEBUG

        return -2;
    }


    

    return 0;
}

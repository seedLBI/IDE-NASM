#ifndef _DEBUG
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif


#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#include "IDE/Application/Application.h"



int main() {
    Application IDE;
    IDE.Run();
    return 0;


    std::setlocale(LC_ALL, "russian");

    Compiler_Nasm compiler(L"tools\\nasm\\nasm.exe");
    //compiler.DisableTranslate();
    compiler.EnableTranslate();
    compiler.EnableOutputFile();
    compiler.EnableDebug();
    compiler.Set_Arg_Architecture(Compiler_ARGS_ARCHITECTURE::Windows64);
    compiler.Set_Arg_Optimization(Compiler_ARGS_OPTIMIZATION::NoOptimization);


    Linker_GCC linker(L"tools\\linker\\MinGW\\bin\\gcc.exe", L"tools\\linker\\MinGW64\\bin\\gcc.exe");
    linker.SetEntryPoint(L"start");
    linker.EnableDebug();

    Debugger_GDB debugger(L"tools\\debugger\\bin\\gdb.exe");

    std::cout << "NASM version:" << std::endl;
    std::cout << "[" << compiler.GetVersion() << "]" << std::endl;

    std::cout << "GDB version:" << std::endl;
    std::cout << "[" << debugger.GetVersion() << "]" << std::endl;
    

    std::wstring NameFile = L"test";

    
    
    
    //std::cout << "Compile" << std::endl;


    //compiler.Compile(NameFile);

    //while (compiler.IsRunning()) {
    //
    //}

    //auto data_output = compiler.GetOutput();

    //for (int i = 0; i < data_output.size(); i++) {
    //    std::cout << convertText_utf8_toCP1251(data_output[i]) << "\n";
    //}

    //std::cout << "Linking" << std::endl;
    //linker.Link(L"test.o", L"test.exe");

    //while (linker.IsRunning()) {
    //
    //}
    //data_output = linker.GetOutput();

    //for (int i = 0; i < data_output.size(); i++) {
    //    std::cout << "[" << convertText_utf8_toCP1251(data_output[i]) << "]\n";
    //}



    
    ParserListingFile parser_listingfile;
    parser_listingfile.Parse(L"WarningsErrorsTest\\Test\\test.lst",L"test");

    debugger.SetListingInfo(parser_listingfile.GetOutput());
    debugger.SetEntryPoint(L"main");
    

    std::cout << "Debugging" << std::endl;

    debugger.Debug(L"WarningsErrorsTest\\Test\\test.exe");

    

    while (debugger.IsRunning()) {

        auto data_output = debugger.GetOutput();

        for (int i = 0; i < data_output.size(); i++) {
            std::cout << "[" << convertText_utf8_toCP1251(data_output[i]) << "]\n";
        }



        static bool bool_pressed = false;

        if (GetAsyncKeyState(char('Q'))) {
            if (bool_pressed == false){
                std::cout << "Q" << std::endl;
                debugger.StepInstruction();
                bool_pressed = true;
            }
        }
        else if (GetAsyncKeyState(char('W'))) {
            if (bool_pressed == false){
                std::cout << "W" << std::endl;
                debugger.Exit();
                bool_pressed = true;
            }
        }
        else if (GetAsyncKeyState(char('E'))) {
            if (bool_pressed == false) {
                std::cout << "E" << std::endl;
                
                bool_pressed = true;

            }
        }
        else if (GetAsyncKeyState(char('R'))) {
            if (bool_pressed == false) {
                std::cout << "R" << std::endl;
                bool_pressed = true;
            }
        }
        else {
            bool_pressed = false;
        }


    }
    


    return 0;
    

}
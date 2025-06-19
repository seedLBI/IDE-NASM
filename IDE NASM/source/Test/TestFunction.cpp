#include "TestFunction.h"

void Test_CompilerTranslate() {

    Compiler_Nasm compiler(L"tools\\nasm\\nasm.exe");
    compiler.EnableTranslate();
    compiler.DisableOutputFile();
    compiler.EnableDebug();
    compiler.Set_Arg_Architecture(Compiler_ARGS_ARCHITECTURE::Windows64);
    compiler.Set_Arg_Optimization(Compiler_ARGS_OPTIMIZATION::NoOptimization);
    std::string path = "WarningsErrorsTest/";

    for (const auto& entry : std::filesystem::directory_iterator(path)) {

        std::wstring path = std::wstring(entry.path()).substr(0, std::wstring(entry.path()).size() - 4);



        compiler.Compile(path.c_str());

        while (compiler.IsRunning()) {
            //if (GetAsyncKeyState(char('E')))
            //    compiler.Terminate();
        }

        auto data_output = compiler.GetOutput();

        for (int i = 0; i < data_output.size(); i++) {
            if (data_output[i].empty())
                continue;
            std::cout << convertText_utf8_toCP1251(data_output[i]) << "\n";
        }

    }


}
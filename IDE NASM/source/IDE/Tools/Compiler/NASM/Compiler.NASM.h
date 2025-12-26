#ifndef COMPILER_NASM_H
#define COMPILER_NASM_H

#include "IDE/Tools/Compiler/Compiler.h"
#include "Utils/Synchronization/CriticalSection/CriticalSection.h"
#include "Utils/ProcessRuner/ProcessRuner.h"

#include "Utils/File/Utils.File.h"
#include "Utils/Text/Encoding/UTF8/Utils.Encoding.UTF8.h"
#include "Utils/Text/Utils.Text.h"
#include <stack>


#include "IDE/Tools/Compiler/NASM/Parser_OutputNasm/NASM.Parser.Output.h"

enum Compiler_ARGS_ARCHITECTURE {
	Windows32,
	Windows64,
};
enum Compiler_ARGS_ERRORS_WARNINGS {
	TranslateWarningToError,
	DisableWarning,
	EnableAll,
	EnableLabelOrphan, // пустые метки или осиротевшие
	Nothing
};
enum Compiler_ARGS_OPTIMIZATION {
	NoOptimization,
	MinimalOptimization,
	MultipassOptimization,
};


class Compiler_Nasm : public Compiler  {
public:
	Compiler_Nasm(const std::wstring& path2compiler);
	~Compiler_Nasm();

	void Terminate() override;
	void Compile(const std::wstring& path2input_file) override;

	std::string GetVersion() override;
	std::vector<std::string> GetOutput() override;

	bool IsCompiled() override;
	bool IsRunning();
	bool HaveError();
	bool HaveWarning();

	void Set_Arg_Architecture (const Compiler_ARGS_ARCHITECTURE&	arg);
	void Set_Arg_ErrorsWarning(const Compiler_ARGS_ERRORS_WARNINGS&	arg);
	void Set_Arg_Optimization (const Compiler_ARGS_OPTIMIZATION&	arg);

	void SetOutputPath(const std::wstring& outputPath);
	void SetNameFile(const std::wstring& NameFile);
	void SetIncludePath(const std::wstring& outputPath);
	void SetDirectoryForRun(const std::wstring directoryPath);

	std::wstring CreateArgumentsLine();

	bool Get_flag_Debug();
	void EnableDebug();
	void DisableDebug();

	bool Get_flag_OutputFile();
	void EnableOutputFile();
	void DisableOutputFile();

	bool Get_flag_Translate();
	void EnableTranslate();
	void DisableTranslate();

private:
	Compiler_ARGS_ARCHITECTURE    arg_architecture    = Windows64;
	Compiler_ARGS_ERRORS_WARNINGS arg_errors_warnings = EnableAll;
	Compiler_ARGS_OPTIMIZATION    arg_optimization    = MultipassOptimization;

	std::string PreDefineMacros;

	std::wstring Path2_InputFile;
	std::wstring OutputFilePath;
	std::wstring NameFile;
	std::wstring IncludePath;
	std::wstring DirectoryPath;

	bool flag_OutputFile = true;
	bool flag_Debug = true;
	bool flag_OutputTranslate = false;

	std::vector<std::string> OutputCompiler;
	std::vector<std::string> OutputCompiler_Processed;


	bool UpdatedAfterCompiled = false;
	void UpdateAfterCompiled();


	bool flagCompiler_HaveError = false;
	bool flagCompiler_HaveWarning = false;


	std::string Version;
	void GetVersionFromExecutable();


	ProcessRuner* CompilerRunner = nullptr;

	std::wstring GetFormatFile_Arg();
	std::wstring GetInputFile_Arg();
	std::wstring GetDebug_Arg();
	std::wstring GetErrorsWarning_Arg();
	std::wstring GetOutputFile_Arg();
	std::wstring GetIncludePath_Arg();
	std::wstring GetOptimization_Arg();
};




#endif //!COMPILER_NASM_H
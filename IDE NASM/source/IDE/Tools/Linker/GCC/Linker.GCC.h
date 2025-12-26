#ifndef LINKER_GCC_H
#define LINKER_GCC_H

#include "Utils/ProcessRuner/ProcessRuner.h"
#include "IDE/Tools/Linker/Linker.h"

#include "Utils/File/Utils.File.h"
#include "Utils/Text/Encoding/Wstring/Utils.Encoding.Wstring.h"



enum class LinkerArchs {
	Windows32,
	Windows64
};

class Linker_GCC 
{
public:
	Linker_GCC(const std::wstring& path2linker_32, const std::wstring& path2linker_64);
	~Linker_GCC();

	void Terminate();
	void Link();
	std::string GetVersion();
	std::vector<std::string> GetOutput();
	bool IsLinked();
	bool IsRunning();


	void SetBitArch(const LinkerArchs& arch);
	void SetPath_ObjectFile(const std::wstring& path);
	void SetPath_OutputExecutableFile(const std::wstring& path);
	void SetDirectoryForRun(const std::wstring directoryPath);
	void SetEntryPoint(const std::wstring& newEntryPoint);


	void EnableDebug();
	void DisableDebug();

	std::wstring MakeArgumentLine();

private:
	ProcessRuner* LinkerRunner_64 = nullptr;
	ProcessRuner* LinkerRunner_32 = nullptr;

	LinkerArchs CurrentArch = LinkerArchs::Windows64;

	std::wstring Path2object_file;
	std::wstring Path2output_executable_file;
	std::wstring PathDirectoryPath;

	std::wstring CustomEntryPoint = L"start";
	bool flag_EnableCustomEntryPoint = true;

	bool flag_EnableDebug = false;


	std::string Version_32;
	std::string Version_64;

	void GetVersionFromExecutable();

	std::wstring Get_arg_input_object_file();
	std::wstring Get_arg_output_file();
	std::wstring Get_arg_custom_entry_point();
	std::wstring Get_arg_debug();

	std::wstring Get_arg_arch();


	std::wstring Get_arg_path_to_base_folder();


	
};



#endif
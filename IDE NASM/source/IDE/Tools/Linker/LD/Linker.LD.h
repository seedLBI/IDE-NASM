#ifndef LD_H
#define LD_H

#include "Utils/ProcessRuner/ProcessRuner.h"
#include "IDE/Tools/Linker/Linker.h"

class Linker_LD : public Linker
{
public:
	Linker_LD();
	~Linker_LD();

	void Link(const std::wstring& path2object_file, const std::wstring& path2output_executable_file) override;
	std::string GetVersion() override;
	std::vector<std::string> GetOutput() override;
	bool IsLinked() override;

	bool IsRunning();

private:
	ProcessRuner* LinkerRunner = nullptr;

	std::wstring Path2object_file;
	std::wstring Path2output_executable_file;


	std::string Version;

	void GetVersionFromExecutable();

	std::wstring Get_arg_input_object_file();
	std::wstring Get_arg_output_file();


	std::wstring MakeArgumentLine();

};


#endif
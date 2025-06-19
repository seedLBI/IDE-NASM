#include "Linker.LD.h"

bool Linker_LD::IsRunning() {
	return LinkerRunner->IsRunning();
}
bool Linker_LD::IsLinked() {
	return LinkerRunner->IsRunning() == false;
}


std::wstring Linker_LD::MakeArgumentLine() {
	return Get_arg_output_file() + Get_arg_input_object_file() + L"-lkernel32 -lmsvcrt --subsystem console -e _start";
}

std::wstring Linker_LD::Get_arg_input_object_file() {
	return Path2object_file + L" ";
}
std::wstring Linker_LD::Get_arg_output_file() {
	return L"-o " + Path2output_executable_file + L" ";
}




std::vector<std::string> Linker_LD::GetOutput() {
	auto output = LinkerRunner->GetOutput();

	/*
	for (auto& line : output) {
		line.pop_back();
	}
	*/

	return output;
}

void Linker_LD::Link(const std::wstring& path2object_file, const std::wstring& path2output_executable_file) {
	Path2object_file = path2object_file;
	Path2output_executable_file = path2output_executable_file;

	LinkerRunner->Run(MakeArgumentLine());
}

void Linker_LD::GetVersionFromExecutable() {
	LinkerRunner->Run(L" -v ");
	while (LinkerRunner->IsRunning() == true) {

	}

	if (LinkerRunner->IsOutputChanged()) {
		Version = LinkerRunner->GetOutput().front();
	}
}

std::string Linker_LD::GetVersion() {
	return Version;
}



Linker_LD::Linker_LD() : Linker(L"tools\\linker\\ld.exe") {
	LinkerRunner = new ProcessRuner(path2linker);
	GetVersionFromExecutable();
}

Linker_LD::~Linker_LD() {

}
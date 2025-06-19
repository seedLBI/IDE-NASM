#include "Linker.GCC.h"

void Linker_GCC::GetVersionFromExecutable() {

	LinkerRunner_32->Run(L" -v ");
	LinkerRunner_64->Run(L" -v ");
	while (LinkerRunner_32->IsRunning()) { }
	while (LinkerRunner_64->IsRunning()) { }

	if (LinkerRunner_32->IsOutputChanged()) {
		Version_32 = LinkerRunner_32->GetOutput().back();
		Version_32.pop_back();
	}
	if (LinkerRunner_64->IsOutputChanged()) {
		Version_64 = LinkerRunner_64->GetOutput().back();
		Version_64.pop_back();
	}
}

std::wstring Linker_GCC::Get_arg_custom_entry_point() {
	if (flag_EnableCustomEntryPoint) {
		return L"-Wl,-e," + CustomEntryPoint + L" -nostartfiles ";
	}
	return L"";
}

std::wstring Linker_GCC::Get_arg_debug() {
	if (flag_EnableDebug) {
		return L"-g ";
	}
	return L"";
}


void Linker_GCC::SetEntryPoint(const std::wstring& newEntryPoint) {
	CustomEntryPoint = newEntryPoint;
}

void Linker_GCC::EnableDebug() {
	flag_EnableDebug = true;
}
void Linker_GCC::DisableDebug() {
	flag_EnableDebug = false;
}


std::wstring Linker_GCC::Get_arg_input_object_file() {
	return L"\"" + Path2object_file + L"\" ";
}

std::wstring Linker_GCC::Get_arg_output_file() {
	return L"-o \"" + Path2output_executable_file + L"\" ";
}

std::wstring Linker_GCC::Get_arg_path_to_base_folder() {
	return L"-B \"" + GetPathDirectory() + L"\\tools\\linker\\libexec\\gcc\\x86_64-w64-mingw32\\14.2.0" + L"\" ";
}

std::wstring Linker_GCC::Get_arg_arch() {
	if (CurrentArch == LinkerArchs::Windows32)
		return L"-m32 ";
	else if (CurrentArch == LinkerArchs::Windows64)
		return L"-m64 ";
}

std::wstring Linker_GCC::MakeArgumentLine() {
	return Get_arg_input_object_file() + Get_arg_custom_entry_point() + Get_arg_output_file() + Get_arg_debug() + Get_arg_arch();
}

void Linker_GCC::SetPath_ObjectFile(const std::wstring& path) {
	Path2object_file = path;
}
void Linker_GCC::SetPath_OutputExecutableFile(const std::wstring& path) {
	Path2output_executable_file = path;
}
void Linker_GCC::SetDirectoryForRun(const std::wstring directoryPath) {
	PathDirectoryPath = directoryPath;
}


void Linker_GCC::Terminate() {
	LinkerRunner_32->Terminate();
	LinkerRunner_64->Terminate();
}


void Linker_GCC::Link() {
#ifdef _DEBUG
	std::wcout << L"[" << MakeArgumentLine() << L"]" << std::endl;
#endif // _DEBUG


	if (CurrentArch == LinkerArchs::Windows32) {
		if (PathDirectoryPath.empty())
			LinkerRunner_32->Run(MakeArgumentLine());
		else
			LinkerRunner_32->Run(MakeArgumentLine(), PathDirectoryPath);
	}
	else if (CurrentArch == LinkerArchs::Windows64){
		if (PathDirectoryPath.empty())
			LinkerRunner_64->Run(MakeArgumentLine());
		else
			LinkerRunner_64->Run(MakeArgumentLine(), PathDirectoryPath);
	}


}



void Linker_GCC::SetBitArch(const LinkerArchs& arch) {
	CurrentArch = arch;
}


std::vector<std::string> Linker_GCC::GetOutput() {

	if (CurrentArch == LinkerArchs::Windows32)
		return LinkerRunner_32->GetOutput();
	else if (CurrentArch == LinkerArchs::Windows64)
		return LinkerRunner_64->GetOutput();

}

bool Linker_GCC::IsRunning() {
	if (CurrentArch == LinkerArchs::Windows32)
		return LinkerRunner_32->IsRunning();
	else if (CurrentArch == LinkerArchs::Windows64)
		return LinkerRunner_64->IsRunning();
}

bool Linker_GCC::IsLinked() {
	if (CurrentArch == LinkerArchs::Windows32)
		return LinkerRunner_32->IsRunning() == false;
	else if (CurrentArch == LinkerArchs::Windows64)
		return LinkerRunner_64->IsRunning() == false;
}
std::string Linker_GCC::GetVersion() {
	if (CurrentArch == LinkerArchs::Windows32)
		return Version_32;
	else if (CurrentArch == LinkerArchs::Windows64)
		return Version_64;
}

Linker_GCC::Linker_GCC(const std::wstring& path2linker_32, const std::wstring& path2linker_64) {
	LinkerRunner_32 = new ProcessRuner(path2linker_32);
	LinkerRunner_64 = new ProcessRuner(path2linker_64);

	GetVersionFromExecutable();
}

Linker_GCC::~Linker_GCC() {

}
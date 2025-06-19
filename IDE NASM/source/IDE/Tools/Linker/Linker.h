#ifndef LINKER_H
#define LINKER_H

#include <string>
#include <vector>

class Linker
{
public:
	Linker(const std::wstring& path2linker);
	~Linker();

	virtual void Link(const std::wstring& path2object_file, const std::wstring& path2output_executable_file) = 0;
	virtual std::vector<std::string> GetOutput() = 0;
	virtual bool IsLinked() = 0;
	virtual std::string GetVersion() = 0;

protected:
	std::wstring path2linker;
};



#endif
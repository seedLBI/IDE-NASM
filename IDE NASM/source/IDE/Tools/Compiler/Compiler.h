#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <vector>

class Compiler
{
public:
	Compiler(const std::wstring& path2compiler);
	~Compiler();

	virtual void Terminate() = 0;
	virtual void Compile(const std::wstring& path2input_file) = 0;
	virtual std::string GetVersion() = 0;
	virtual std::vector<std::string> GetOutput() = 0;
	virtual bool IsCompiled() = 0;

protected:
	std::wstring path2compiler;
};


#endif
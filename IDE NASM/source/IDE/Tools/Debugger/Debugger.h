#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <string>
#include <vector>


class Debugger {
public:

	virtual void Debug(const std::wstring& path2debug) = 0;
	virtual std::vector<std::string> GetOutput() = 0;
	virtual std::string GetVersion() = 0;
};


#endif // !DEBUGGER_H
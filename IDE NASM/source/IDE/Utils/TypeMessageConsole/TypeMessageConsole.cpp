#include "TypeMessageConsole.h"


TypeMessageConsole STR_to_TypeMessageConsole(const std::string& str_type) {
	if (str_type == "nasm")
		return MessageConsole_Compiler;

	if (str_type == "parser")
		return MessageConsole_Parser;

	if (str_type == "panic")
		return MessageConsole_Panic;

	if (str_type == "info")
		return MessageConsole_Info;

	if (str_type == "fatal")
		return MessageConsole_Fatal;

	if (str_type == "error")
		return MessageConsole_Error;

	if (str_type == "warning")
		return MessageConsole_Warning;

	if (str_type == "success")
		return MessageConsole_Success;

	exit(-10010);
	return MessageConsole_Error;
}

std::string TypeMessageConsole_to_STR(const TypeMessageConsole& type) {
	switch (type) {
		case MessageConsole_Compiler:	return "nasm";
		case MessageConsole_Info:		return "info";
		case MessageConsole_Warning:	return "warning";
		case MessageConsole_Error:		return "error";
		case MessageConsole_Panic:		return "panic";
		case MessageConsole_Parser:		return "parser";
		case MessageConsole_Fatal:		return "fatal";
		case MessageConsole_Success:	return "success";
	default: return "NOTHING";
	}
}
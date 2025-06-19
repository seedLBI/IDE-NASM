#ifndef TYPE_MESSAGE_CONSOLE_H
#define TYPE_MESSAGE_CONSOLE_H


#include <string>


enum TypeMessageConsole {
	MessageConsole_Compiler,
	MessageConsole_Info,
	MessageConsole_Warning,
	MessageConsole_Error,
	MessageConsole_Panic,
	MessageConsole_Parser,
	MessageConsole_Fatal,
	MessageConsole_Success,
};

TypeMessageConsole STR_to_TypeMessageConsole(const std::string& str_type);
std::string TypeMessageConsole_to_STR(const TypeMessageConsole& type);


#endif // !TYPE_NOTIFICATION_CONSOLE_H

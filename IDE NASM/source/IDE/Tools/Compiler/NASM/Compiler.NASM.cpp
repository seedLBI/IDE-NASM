#include "Compiler.NASM.h"


Compiler_Nasm::Compiler_Nasm(const std::wstring& path2compiler) : Compiler(path2compiler) {
	CompilerRunner = new ProcessRuner(path2compiler);

	GetVersionFromExecutable();
}
Compiler_Nasm::~Compiler_Nasm() {

}

void Compiler_Nasm::SetOutputPath(const std::wstring& outputPath) {
	OutputFilePath = outputPath;
}
void Compiler_Nasm::SetNameFile(const std::wstring& NameFile) {
	this->NameFile = NameFile;
}

void Compiler_Nasm::SetIncludePath(const std::wstring& includePath) {
	IncludePath = includePath;
}
void Compiler_Nasm::SetDirectoryForRun(const std::wstring directoryPath) {
	DirectoryPath = directoryPath;
}

void Compiler_Nasm::Set_Arg_Architecture(const Compiler_ARGS_ARCHITECTURE& arg) {
	arg_architecture = arg;
}
void Compiler_Nasm::Set_Arg_ErrorsWarning(const Compiler_ARGS_ERRORS_WARNINGS& arg) {
	arg_errors_warnings = arg;
}
void Compiler_Nasm::Set_Arg_Optimization(const Compiler_ARGS_OPTIMIZATION& arg) {
	arg_optimization = arg;
}

std::wstring Compiler_Nasm::GetFormatFile_Arg() {
	switch (arg_architecture) {
	case Windows32:
		return L"-f win32 ";
		break;
	case Windows64:
		return L"-f win64 ";
		break;
	default:
		break;
	}
}
std::wstring Compiler_Nasm::GetOptimization_Arg() {
	std::wstring result;

	switch (arg_optimization)
	{
	case Compiler_ARGS_OPTIMIZATION::NoOptimization:
		result += L"-O0 ";
		break;
	case Compiler_ARGS_OPTIMIZATION::MinimalOptimization:
		result += L"-O1 ";
		break;
	case Compiler_ARGS_OPTIMIZATION::MultipassOptimization:
		result += L"-Ox ";
		break;
	default:
		result += L"-O1 ";
		break;
	}

	if (flag_Debug)
		result += L"-Ov ";

	return result;
}
std::wstring Compiler_Nasm::GetErrorsWarning_Arg() {
	switch (arg_errors_warnings)
	{
	case TranslateWarningToError:
		return L"-w-error ";
		break;
	case DisableWarning:
		return L"-e ";
		break;
	case EnableAll:
		return L"-w+all ";
		break;
	case EnableLabelOrphan:
		return L"-w+label-orphan ";
		break;
	case Nothing:
		return L"";
		break;
	default:
		break;
	}
}
std::wstring Compiler_Nasm::GetDebug_Arg() {
	if (flag_Debug) {
		std::wstring output;
		output += L"-g ";


		if (flag_OutputFile) {
			output += L"-F cv8 ";
			if (OutputFilePath.empty())
				output += L"-l " + Path2_InputFile + L".lst ";
			else
				return L"-l \"" + OutputFilePath + L"\\" + NameFile + L".lst\" ";
		}
		

		return output;
	}
	else
		return L"";
}
std::wstring Compiler_Nasm::GetInputFile_Arg() {
	return L"\"" + Path2_InputFile + L"\" ";
}
std::wstring Compiler_Nasm::GetOutputFile_Arg() {
	if (flag_OutputFile) {
		if (OutputFilePath.empty())
			return L"-o \"" + Path2_InputFile + L".o\" ";
		else
			return L"-o \"" + OutputFilePath + L"\\" + NameFile + L".o\" ";
		
	}
	return L"";
}
std::wstring Compiler_Nasm::GetIncludePath_Arg() {
	if (IncludePath.empty() == false)
		return L"-i \"" + IncludePath + L"\" ";
	return L"";
}


std::wstring Compiler_Nasm::CreateArgumentsLine() {

	return
		GetFormatFile_Arg() +
		GetInputFile_Arg() +
		GetOutputFile_Arg() +
		GetIncludePath_Arg() +
		GetErrorsWarning_Arg() +
		GetOptimization_Arg() +
		GetDebug_Arg();

}


bool Compiler_Nasm::Get_flag_Debug() {
	return flag_Debug;
}
void Compiler_Nasm::EnableDebug() {
	flag_Debug = true;
}
void Compiler_Nasm::DisableDebug() {
	flag_Debug = false;
}


bool Compiler_Nasm::Get_flag_OutputFile() {
	return flag_OutputFile;
}
void Compiler_Nasm::EnableOutputFile() {
	flag_OutputFile = true;
}
void Compiler_Nasm::DisableOutputFile() {
	flag_OutputFile = false;
}


bool Compiler_Nasm::Get_flag_Translate() {
	return flag_OutputTranslate;
}
void Compiler_Nasm::EnableTranslate() {
	flag_OutputTranslate = true;
}
void Compiler_Nasm::DisableTranslate() {
	flag_OutputTranslate = false;
}


void Compiler_Nasm::Terminate() {
	CompilerRunner->Terminate();
}
void Compiler_Nasm::Compile(const std::wstring& path2input_file) {
	UpdatedAfterCompiled = false;
	Path2_InputFile = path2input_file;

	if (DirectoryPath.empty() == false)
		CompilerRunner->Run(CreateArgumentsLine(), DirectoryPath);
	else
		CompilerRunner->Run(CreateArgumentsLine());
}

std::string Compiler_Nasm::GetVersion() {
	return Version;
}
void Compiler_Nasm::GetVersionFromExecutable() {
	CompilerRunner->Run(L" -v ");

	while (CompilerRunner->IsRunning()) {
		Sleep(10);
	}

	auto data = CompilerRunner->GetOutput();

	if (!data.empty()) {
		Version = data.front();

		if (!Version.empty() && (Version.back() == '\n' || Version.back() == '\r')) {
			Version.pop_back();
		}
	}

}

std::vector<std::string> Compiler_Nasm::GetOutput() {
	UpdateAfterCompiled();
	return OutputCompiler_Processed;
}

bool Compiler_Nasm::IsRunning() {
	return CompilerRunner->IsRunning();
}
bool Compiler_Nasm::IsCompiled() {
	return CompilerRunner->IsRunning() == false;
}

bool Compiler_Nasm::HaveError() {
	UpdateAfterCompiled();
	return flagCompiler_HaveError;
}
bool Compiler_Nasm::HaveWarning() {
	UpdateAfterCompiled();
	return flagCompiler_HaveWarning;
}

void Compiler_Nasm::UpdateAfterCompiled() {
	if (UpdatedAfterCompiled == true)
		return;


	OutputCompiler = CompilerRunner->GetOutput();
	OutputCompiler_Processed = OutputCompiler;

	for (auto& line : OutputCompiler_Processed) {
		line.pop_back();

		//Translate(line);
	}

	UpdatedAfterCompiled = true;
}
/*
void Compiler_Nasm::Translate(std::string& line) {

	if (line.empty())
		return;


	const static std::vector<std::pair<std::string, std::string>> TranslationData = {
			{"label alone on a line without a colon might be in error",
			u8"Метка в строке без двоеточия может быть ошибочной"},

			{"label or instruction expected at start of line",
			u8"В начале строки ожидается метка или инструкция"},
			
			{"integer supplied as 128-bit data",
			u8"Целое число, представленное в виде 128-битных данных"},

			{"invalid 64-bit effective address",
			u8"Недопустимый 64-битный эффективный адрес"},

			{"invalid effective address: too many registers",
			u8"Недопустимый эффективный адрес: слишком много регистров"},

			{"signed dword immediate exceeds bounds",
			u8"Непосредственное значение типа <signed dword> выходит за допустимые границы"},

			{"byte data exceeds bounds",
			u8"Значение типа <byte> выходит за допустимые границы"},

			{"dword data exceeds bounds",
			u8"Значение типа <dword> выходит за допустимые границы"},

			{"word data exceeds bounds",
			u8"Значение типа <word> выходит за допустимые границы"},

			{"-bit relative section-crossing relocation",
			u8"-битное относительное перемещение пересекающихся секций"},

			{"-bit absolute section-crossing relocation",
			u8"-битное абсолютное перемещение пересекающихся секций"},

			{"instruction has redundant prefixes",
			u8"Инструкция имеет лишние префиксы"},

			{"invalid combination of opcode and operands",
			u8"Операнды не подходят к указанной инструкции"},

			{"comma, colon, decorator or end of line expected after operand",
			u8"После операнда ожидается запятая, двоеточие, декоратор или конец строки"},

			{"comma, decorator or end of line expected, got",
			 u8"Ожидается запятая, декоратор или конец строки, получено"},

			{"no operand for data declaration",
			u8"Нет операнда для объявления данных"},

			{"comma expected after operand, got `",
			u8"После операнда ожидалась запятая, получено `"},

			{"segment alignment `",
			u8"Выравнивание сегмента: `"},

			{"is not power of two",
			u8"не является степенью двойки"},

			{"non-constant argument supplied to",
			u8"Неконстантный аргумент, передаваемый в"},

			{"more than one input file specified",
			u8"Указано более одного входного файла"},

			{"unterminated string (missing `",
			u8"Непрерванная строка (отсутствует `"},

			{"assembly required",
			u8"Сборка потребовала"},

			{"passes",
			u8"проходов"},

			{"expecting `",
			u8"Ожидается `"},

			{"expected `",
			u8"Ожидался `"},

			{"trailing garbage after `",
			u8"Мусор после `"},
			
			{"ignored",
			u8"игнорируется"},

			{"before end of file",
			u8"до конца файла"},

			{"end of file while still defining macro",
			u8"Вплоть до конца файла в ожидании конца определения макроса"},

			{"is not a valid decorator with braces",
			u8"Не является допустимым декоратором с фигурными скобками"},

			{"EQU not preceded by label",
			u8"Ожидается метка перед EQU"},

			{"expects a macro identifier",
			u8"ожидает идентификатор макроса"},

			{"expects a parameter count",
			u8"ожидает параметр count"},

			{"unable to open include file",
			u8"Невозможно открыть включаемый файл"},

			{"unable to open input file",
			u8"Невозможно открыть входной файл"},

			{"No such file or directory",
			u8"Нет такого файла или каталога"},

			{"division by zero",
			u8"Деление на ноль"},

			{"expression syntax error",
			u8"Cинтаксическая ошибка выражения"},

			{"instruction expected",
			u8"Ожидалась инструкция"},

			{"operation size not specified",
			u8"Не указан размер операции"},

			{"broadcast not permitted on this operand",
			u8"Broadcast не разрешён для данного операнда"},

			{"instruction not supported in 32-bit mode",
			u8"Инструкция не поддерживается в 32-битном режиме"},

			{"expecting ] at end of memory operand",
			u8"Ожидается ] в конце операдна памяти"},

			{"uninitialised space declared in non-BSS section `",
			u8"Неинициализированная область объявлена в не-BSS секции `"},

			{"': zeroing",
			u8"': выполнено обнуление"},

			{"Permission denied",
			u8"В доступе отказано"},

			{"not defining a macro",
			u8"используется вне определения макроса"},

			{"from macro",
			u8"Из макроса"},

			{"redefining multi-line macro",
			u8"Переопределение многострочного макроса"},

			{"multi-line macro",
			u8"Многострочный макрос"},

			{"exists, but not taking 1 parameter",
			u8"существует, но не принимает 1 параметр"},

			{"exists, but not taking 0 parameters",
			u8"существует, но не принимает 0 параметров"},

			{"inconsistently redefined",
			u8"непоследовательное переопределение"},

			{"originally defined here",
			u8"первоначально определена здесь"},

			{"does not fit in 64 bits",
			u8"Не поместится в 64 бита"},

			{"defined here",
			u8"определенного здесь"},

			{"symbol",
			u8"Символ"},

			{"label `",
			u8"Метка `"},

			{"numeric constant",
			u8"Числовая константа"},

			{"not defined",
			u8"не определён"},

	};


	auto IsAllNumbers = [](const std::string& text) {
		
		for (const auto& symbol : text) {
			bool isNumber = symbol >= '0' && symbol <= '9';
			if (isNumber == false)
				return false;

		}
		return true;
	};


	std::string temp = line;

	std::string FileName, PosLineCode, TypeNotification, TextNotification, FlagWarning;

	FileName = temp.substr(0, temp.find_first_of(':'));
	if (FileName.size() == 1) {
		FileName = temp.substr(0, temp.find_first_of(':',2));
	}
	temp.erase(temp.begin(), temp.begin() + FileName.size() + 1);


	if (FileName == "panic") {
		TypeNotification = "panic";

		FileName = temp.substr(0, temp.find_first_of(':'));
		temp.erase(temp.begin(), temp.begin() + FileName.size() + 1);

		if (FileName.size() > 0 && FileName[0] == ' ')
			FileName.erase(FileName.begin());


		PosLineCode = temp.substr(0, temp.find_first_of(':'));

		if (IsAllNumbers(PosLineCode))
			temp.erase(temp.begin(), temp.begin() + PosLineCode.size() + 2);
		else
			PosLineCode.clear();

	}
	else {
		if (FileName != "nasm") {

			PosLineCode = temp.substr(0, temp.find_first_of(':'));

			if (IsAllNumbers(PosLineCode))
				temp.erase(temp.begin(), temp.begin() + PosLineCode.size() + 2);
			else
				PosLineCode.clear();

		}
		else if (flag_OutputTranslate) {
			FileName = u8"Компилятор";
		}

		TypeNotification = temp.substr(0, temp.find_first_of(':'));

		std::vector<std::string> str_TypeInfo = { "fatal", "error", "warning", "info", "parser" };
		bool ItsNotification = false;
		for (int i = 0; i < str_TypeInfo.size(); i++) {
			if (TypeNotification.find(str_TypeInfo[i]) != std::string::npos) {
				ItsNotification = true;
				break;
			}
		}

		if (ItsNotification) {
			temp.erase(temp.begin(), temp.begin() + TypeNotification.size() + 2);

			if (temp.size() > 7) {
				if (temp.substr(0, 7) == "parser:") {
					TypeNotification += u8": parser";
					temp.erase(temp.begin(), temp.begin() + 8);
				}
			}
		}
		else {
			TypeNotification.clear();
		}
	}


	bool HaveFlag = false;

	auto posBreket = temp.find_last_of('[');

	if (posBreket != std::string::npos) {
		if (temp[posBreket + 1] == '-')
			HaveFlag = true;
	}

	if (HaveFlag) {
		FlagWarning = temp.substr(posBreket, temp.find_last_of(']') - posBreket + 1);
		temp.erase(temp.begin() + posBreket, temp.end());

	}

	TextNotification = temp;

	bool IsPanic   = TypeNotification.find("panic") != std::string::npos;
	bool IsFatal   = TypeNotification.find("fatal") != std::string::npos;
	bool IsError   = TypeNotification.find("error") != std::string::npos;
	bool IsWarning = TypeNotification.find("warning") != std::string::npos;
	bool IsInfo    = TypeNotification.find("info") != std::string::npos;
	bool IsParser  = TypeNotification.find("parser") != std::string::npos;
	bool IsNothing =
		IsPanic == false &&
		IsFatal == false &&
		IsError == false &&
		IsWarning == false &&
		IsInfo == false &&
		IsParser == false;



	if (IsError || IsFatal || IsPanic)
		flagCompiler_HaveError = true;
	if (IsWarning)
		flagCompiler_HaveWarning = true;



	if (flag_OutputTranslate) {

		if      (IsError)   TypeNotification = u8"Ошибка";
		else if (IsFatal)   TypeNotification = u8"Критическая ошибка";
		else if (IsPanic)   TypeNotification = u8"Аварийное завершение";
		else if (IsWarning) TypeNotification = u8"Предупреждение";
		else if (IsInfo)    TypeNotification = u8"Информация";

		if (IsParser) TypeNotification += u8": Парсер";


		for (int i = 0; i < TranslationData.size(); i++) {

			auto pos_find = TextNotification.find(TranslationData[i].first);

			bool Finded = pos_find != std::string::npos;

			if (Finded) {

				TextNotification.replace(
					TextNotification.begin() + pos_find,
					TextNotification.begin() + pos_find + TranslationData[i].first.size(),
					TranslationData[i].second
				);

			}

		}
	}

	// Compositing [line]

	line.clear();
	
	line = FileName + ": ";
	
	if (PosLineCode.empty() == false)
		line += PosLineCode + ": ";

	if (TypeNotification.empty() == false)
		line += TypeNotification + ": ";
		
	line += TextNotification;
	line += FlagWarning;

}

*/
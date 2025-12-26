#include "InfoLine.IDE.Creater.h"
#include "Utils/Time/Utils.Time.h"

InfoLineIDE InfoLineIDE_Creater::MakeInfo_BeginCompile() {
	auto timestamp = GetCurrentTimestamp();
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::compilation_begin;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);
	result.description_additional = Get_str_CurrentTimestamp_OnlyTime();


	CompilationTimer.start();

	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndCompile() {
	CompilationTimer.stop();



	InfoLineIDE result;

	result.message_type = TypeMessageIDE::compilation_end;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);



	auto time_temp = CompilationTimer.elapsedMilliseconds();

	uint64_t timestamp = static_cast<uint64_t>(time_temp);

	int hour = static_cast<int>(timestamp / uint64_t(60 * 60 * 1000));
	int minute = static_cast<int>(timestamp / uint64_t(60 * 1000)) - hour * 60;
	int second = static_cast<int>(timestamp / (uint64_t(1000)) - minute * 60 - hour * 60 * 60);
	int millisecond = static_cast<int>(timestamp - second * 1000 - minute * 60 - hour * 60 * 60);
	

	std::string str_hour = std::to_string(hour);
	std::string str_minute = std::to_string(minute);
	std::string str_second = std::to_string(second);
	std::string str_millisecond = std::to_string(millisecond);

	if (hour > 0)
		result.description_additional += str_hour + u8" час. ";
	if (minute > 0)
		result.description_additional += str_minute + u8" мин. ";
	if (second > 0)
		result.description_additional += str_second + u8" сек. ";
	if (millisecond != 0)
		result.description_additional += str_millisecond + u8" мс. ";
	else
		if (minute == 0 && hour == 0 && second == 0)
			result.description_additional += str_millisecond + u8" мс. ";
	

	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndCompile_Bad_Error(const int& countErrors) {
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::compilation_end_bad_error;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);

	result.description_additional = std::to_string(countErrors);

	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndCompile_Bad_Data_Wrong_Section() {
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::compilation_end_bad_data_wrong_section;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);

	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndCompile_Good() {
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::compilation_end_good;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);

	return result;
}

InfoLineIDE InfoLineIDE_Creater::MakeInfo_BeginLink() {
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::linking_begin;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);
	result.description_additional = Get_str_CurrentTimestamp_OnlyTime();



	LinkTimer.start();
	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndLink() {
	LinkTimer.stop();



	InfoLineIDE result;

	result.message_type = TypeMessageIDE::linking_end;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);



	auto time_temp = LinkTimer.elapsedMilliseconds();

	uint64_t timestamp = static_cast<uint64_t>(time_temp);

	int hour = static_cast<int>(timestamp / uint64_t(60 * 60 * 1000));
	int minute = static_cast<int>(timestamp / uint64_t(60 * 1000)) - hour * 60;
	int second = static_cast<int>(timestamp / (uint64_t(1000)) - minute * 60 - hour * 60 * 60);
	int millisecond = static_cast<int>(timestamp - second * 1000 - minute * 60 - hour * 60 * 60);


	std::string str_hour = std::to_string(hour);
	std::string str_minute = std::to_string(minute);
	std::string str_second = std::to_string(second);
	std::string str_millisecond = std::to_string(millisecond);

	if (hour > 0)
		result.description_additional += str_hour + u8" час. ";
	if (minute > 0)
		result.description_additional += str_minute + u8" мин. ";
	if (second > 0)
		result.description_additional += str_second + u8" сек. ";
	if (millisecond != 0)
		result.description_additional += str_millisecond + u8" мс. ";
	else
		if (minute == 0 && hour == 0 && second == 0)
			result.description_additional += str_millisecond + u8" мс. ";


	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndLink_Good() {
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::linking_end_good;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);

	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndLink_Bad_Error() {
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::linking_end_bad_error;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);

	return result;
}

InfoLineIDE InfoLineIDE_Creater::MakeInfo_BeginRun() {
	auto timestamp = GetCurrentTimestamp();
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::running_begin;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);
	result.description_additional = Get_str_CurrentTimestamp_OnlyTime();


	RunTimer.start();

	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndRun_Good() {
	RunTimer.stop();



	InfoLineIDE result;

	result.message_type = TypeMessageIDE::running_end_good;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);



	auto time_temp = RunTimer.elapsedMilliseconds();

	uint64_t timestamp = static_cast<uint64_t>(time_temp);

	int hour = static_cast<int>(timestamp / uint64_t(60 * 60 * 1000));
	int minute = static_cast<int>(timestamp / uint64_t(60 * 1000)) - hour * 60;
	int second = static_cast<int>(timestamp / (uint64_t(1000)) - minute * 60 - hour * 60 * 60);
	int millisecond = static_cast<int>(timestamp - second * 1000 - minute * 60 - hour * 60 * 60);


	std::string str_hour = std::to_string(hour);
	std::string str_minute = std::to_string(minute);
	std::string str_second = std::to_string(second);
	std::string str_millisecond = std::to_string(millisecond);

	if (hour > 0)
		result.description_additional += str_hour + u8" час. ";
	if (minute > 0)
		result.description_additional += str_minute + u8" мин. ";
	if (second > 0)
		result.description_additional += str_second + u8" сек. ";
	if (millisecond != 0)
		result.description_additional += str_millisecond + u8" мс. ";
	else
		if (minute == 0 && hour == 0 && second == 0)
			result.description_additional += str_millisecond + u8" мс. ";


	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndRun_Info(const DWORD& exitCode) {
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::running_end_info;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);
	result.description_additional = "(" + std::to_string(exitCode) + ")";

	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndRun_No_Exe() {
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::running_no_exe;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);

	return result;
}
InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndRun_Terminated() {
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::running_end_terminated;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);

	return result;
}


InfoLineIDE InfoLineIDE_Creater::MakeInfo_EndDebug_No_Exe() {
	InfoLineIDE result;

	result.message_type = TypeMessageIDE::debugging_no_exe;
	result.GetType(dataTranslate);
	result.GetDescription(dataTranslate);

	return result;
}


InfoLineIDE_Creater::InfoLineIDE_Creater() {
	if (isFileExist(L"resources\\json\\TranslateOutputConsole.json")) {
		std::ifstream ifn(L"resources\\json\\TranslateOutputConsole.json");

		try {
			dataTranslate = nlohmann::json::parse(ifn);
		}
		catch (const nlohmann::json::parse_error& e) {
			static_cast<void>(e);
#ifdef _DEBUG
			std::cout << "ERROR  OutputConsoleDrawer_NASM::OutputConsoleDrawer_NASM()\n";
#endif // _DEBUG

		}

		ifn.close();
	}
	else {
		MessageBoxW(0, L"А где файл переводов? (resources\\json\\TranslateOutputConsole.json)", L"Ошибка загрузки данных", MB_ICONERROR | MB_OK);
		exit(0);
	}

	dataTranslate = dataTranslate["IDE"]["messages"];
}

InfoLineIDE_Creater::~InfoLineIDE_Creater() {

}
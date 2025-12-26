#include "Utils.Time.h"

#include "Utils/Text/Utils.Text.h"

uint64_t GetCurrentTimestamp() {
    std::time_t result = std::time(nullptr);

    return result;
}


std::string Get_str_CurrentTimestamp_OnlyTime() {
	
	time_t ts = static_cast<time_t>(GetCurrentTimestamp());
	struct tm dt;
	localtime_s(&dt, &ts);

	int year = dt.tm_year + 1900;
	int month = dt.tm_mon + 1;
	int day = dt.tm_mday;
	int hour = dt.tm_hour;
	int minute = dt.tm_min;
	int second = dt.tm_sec;

	std::string str_hour = std::to_string(hour);
	PadLeft(str_hour, '0', 2);

	std::string str_minute = std::to_string(minute);
	PadLeft(str_minute, '0', 2);

	std::string str_second = std::to_string(second);
	PadLeft(str_second, '0', 2);

	std::string time = "[" + str_hour + ":" + str_minute + ":" + str_second + "]";

	return time;
}
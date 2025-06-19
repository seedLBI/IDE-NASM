#ifndef INFOLINE_IDE_CREATER_H
#define INFOLINE_IDE_CREATER_H

#include "InfoLine.IDE.h"
#include "Utils/Time/Utils.Time.h"
#include "Utils/Text/Utils.Text.h"
#include "Utils/File/Utils.File.h"

#include "Utils/Timer/Utils.Timer.h"

class InfoLineIDE_Creater {
public:
	InfoLineIDE_Creater();
	~InfoLineIDE_Creater();
	
	InfoLineIDE MakeInfo_BeginCompile();
	InfoLineIDE MakeInfo_EndCompile();
	InfoLineIDE MakeInfo_EndCompile_Bad_Error(const int& countErrors);
	InfoLineIDE MakeInfo_EndCompile_Bad_Data_Wrong_Section();
	InfoLineIDE MakeInfo_EndCompile_Good();


	InfoLineIDE MakeInfo_BeginLink();
	InfoLineIDE MakeInfo_EndLink();
	InfoLineIDE MakeInfo_EndLink_Good();
	InfoLineIDE MakeInfo_EndLink_Bad_Error();

	InfoLineIDE MakeInfo_BeginRun();
	InfoLineIDE MakeInfo_EndRun_Good();
	InfoLineIDE MakeInfo_EndRun_Info(const DWORD& exitCode);
	InfoLineIDE MakeInfo_EndRun_No_Exe();
	InfoLineIDE MakeInfo_EndRun_Terminated();


	InfoLineIDE MakeInfo_EndDebug_No_Exe();



private:
	nlohmann::json dataTranslate;
	Timer CompilationTimer;
	Timer LinkTimer;
	Timer RunTimer;
};





#endif
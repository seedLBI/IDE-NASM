#ifndef STARTUP_WINDOW_H
#define STARTUP_WINDOW_H

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"
#include "ThirdParty/IconFontCppHeaders/IconsFontAwesome6.h"

#include "IDE/Core/FontManager/FontManager.h"
#include "IDE/Core/Solution/SolutionInfo.h"

#include "Utils/File/Utils.File.h"

#include <string>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <nlohmann/json.hpp>


class Solution;

class StartupWindow {
public:
	StartupWindow(FontManager* fontManager, Solution* solution);
	~StartupWindow();

	void SetPTR_last_solutions(std::vector<SolutionInfo>* last_solutions);

	void Draw();
	void Update();

	bool NeedClose();

private:
	FontManager* fontManager = nullptr;
	Solution* solution = nullptr;

	bool isNeedToBeClosed = false;

	// Customizable 
	ImVec4 Color_Favorite;
	ImVec4 Color_Favorite_Hovered;
	ImVec4 Color_Outline;


	bool DrawListObject(SolutionInfo& info, const int& index, bool& btn_pressed_fav, bool& btn_pressed_del, bool& btn_pressed_main);


	int Index_pressed_main = -1,
		Index_pressed_fav = -1,
		Index_pressed_del = -1;

	
	void TryingOpen();
	void TryingOpen(const std::wstring& fullpath);

	void SaveAllSolutions();

	std::vector<SolutionInfo>* last_solutions;

	int CurrentIndexHovered = -1;

	std::string timestamp_to_DateAndTime_str(const uint64_t& timestamp);
	std::string timestamp_to_Time_str(const uint64_t& timestamp);

};

#endif 
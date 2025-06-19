#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Utils/Time/Utils.Time.h"
#include "Utils/Timer/Utils.Timer.h"
#include "Utils/ImGui/ImGui.Utils.h"


#include "IDE/Core/Solution/SolutionInfo.h"
#include "IDE/GUI/Widgets/FilesViewer/Widget_FilesViewer.h"
#include "IDE/GUI/Widgets/TextEditor/WidgetManager_TextEditior.h"

#include "IDE/Core/Widget/WidgetManager.h"

#include "IDE/Core/PositionWidgetsManager/PositionWidgetsManager.h"


class LastSolutionManager;

class Solution {
private:

	const std::string TemplateDefaultMainFile =
R"(global main
section .text

main:
mov al,128
ret)";



public:
	Solution();
	~Solution();

	void Init(WidgetManager_TextEditor* widgetManager_TextEditor, LastSolutionManager* lastSolutionManager, Widget_FilesViewer* widget_FilesViewer, PositionWidgetsManager* positionWidgetsManager, WidgetManager* widgetManager);

	SolutionInfo GetInfo();

	void DrawPopupCreation();


	void SaveCurrentFocusedFile();
	void SaveAllFiles();

	void SaveCurrentSolution();

	void Create();
	bool isCreated();

	bool Open();
	bool OpenFromPath(const std::wstring& CompletePath);

	void Close();

private:
	WidgetManager_TextEditor* widgetManager_TextEditor = nullptr;
	LastSolutionManager* lastSolutionManager = nullptr;
	Widget_FilesViewer* widget_FilesViewer = nullptr;
	PositionWidgetsManager* positionWidgetsManager = nullptr;
	WidgetManager* widgetManager = nullptr;

	SolutionInfo CurrentSolution;

	bool isOpened = false;
	bool flag_isCreated = false;

	bool flag_PopupCreation = false;


	std::wstring PathRepos;
	std::wstring CompletePath;

	Timer Timer_spent;

};



#endif
#ifndef WIDGET_FILES_VIEWER_H
#define WIDGET_FILES_VIEWER_H

#include <nlohmann/json.hpp>
#include <string>

#include "ThirdParty/IconFontCppHeaders/IconsFontAwesome6.h"
#include "Utils/Text/Encoding/Wstring/Utils.Encoding.Wstring.h"

#include "IDE/Core/Widget/IWidget.h"
#include "IDE/Core/Solution/SolutionInfo.h"

#include "IDE/GUI/Widgets/TextEditor/WidgetManager_TextEditior.h"


class Widget_FilesViewer : public IWidget {
public:
	Widget_FilesViewer(WidgetManager_TextEditor* widgetManager_TextEditor);
	~Widget_FilesViewer();

	void LoadSolutionInfo(SolutionInfo* solutionData_ptr);

	void Draw() override;
	void Update() override;
	nlohmann::json Save() override;
	void Load(const nlohmann::json& Data) override;

private:
	SolutionInfo* currentSolution = nullptr;
	nlohmann::json directory_info;

	WidgetManager_TextEditor* widgetManager_TextEditor = nullptr;

	std::vector<std::string> folders_open;

	ImColor color_folder;
	

	void OpenInExplorer(const std::string& path);


	void UpdateDirectory();

	nlohmann::json build_directory_json(const std::wstring& path);
	void render_imgui_tree(const nlohmann::json& node);
};



#endif
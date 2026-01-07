#ifndef WIDGET_MANAGER_TEXT_EDITOR_H
#define WIDGET_MANAGER_TEXT_EDITOR_H

#include "IDE/Core/Widget/IWidget.h"
#include "Widget_TextEditor.h"

#include "Utils/File/Utils.File.h"
#include "Utils/Text/Utils.Text.h"

#include "IDE/Core/Theme/interface/IThemeLoadable.h"

#include "IDE/Core/Solution/SolutionInfo.h"

#include "IDE/Core/PositionWidgetsManager/PositionWidgetsManager.h"

#include <vector>
#include <nlohmann/json.hpp>

class WidgetManager_TextEditor : public IWidget, public IThemeLoadable {
public:
	WidgetManager_TextEditor(FPS_Timer* fps_limiter, PositionWidgetsManager* positionsWidgetsManager);
	~WidgetManager_TextEditor();

	void Draw() override;
	void Update() override;

	std::vector<std::string> GetNamesTextEditorChild();

	Widget_TextEditor* GetFocusedTextEditor();
	Widget_TextEditor* GetLastFocusedTextEditor();

	void LoadSolutionInfo(SolutionInfo* solutionData_ptr);


	std::vector<Widget_TextEditor*> GetWidgets();


	void LoadColors() override;
	std::vector<NamedColor> GetDefaultLightColors() override;
	std::vector<NamedColor> GetDefaultDarkColors() override;


	nlohmann::json Save() override;
	void Load(const nlohmann::json& Data) override;

	void Add(const std::wstring& Path);

	void Remove(const std::wstring& Path);


	void CloseAll();
	
	void ClearAll();
	void SetActiveFromPath(const std::wstring& Path);

	void Highlight_error(const std::wstring& Path, int line);
	void Highlight_warning(const std::wstring& Path, int line);
	void Highlight_info(const std::wstring& Path, int line);


	void SaveCurrentActiveFile();
	void SaveAllFile();

	Widget_TextEditor* GetWidgetFromPath(const std::wstring& Path);

private:
	SolutionInfo* currentSolution = nullptr;
	FPS_Timer* fps_limiter = nullptr;
	PositionWidgetsManager* positionsWidgetsManager = nullptr;

	std::string LastActiveWidget;

	std::vector<Widget_TextEditor*> widgets;

	ImColor color_highlight_error = ImColor(1.f,0.f,0.f,1.f);
	ImColor color_highlight_warning = ImColor(1.f, 1.f, 0.f, 1.f);
	ImColor color_highlight_info = ImColor(0.f, 0.f, 1.f, 1.f);



};



#endif
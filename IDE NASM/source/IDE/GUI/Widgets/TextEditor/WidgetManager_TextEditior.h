#ifndef WIDGET_MANAGER_TEXT_EDITOR_H
#define WIDGET_MANAGER_TEXT_EDITOR_H

#include "IDE/Core/Widget/IWidget.h"
#include "Widget_TextEditor.h"

#include "Utils/File/Utils.File.h"
#include "IDE/Core/Theme/interface/IThemeLoadable.h"

#include <vector>
#include <nlohmann/json.hpp>

class WidgetManager_TextEditor : public IWidget, public IThemeLoadable {
public:
	WidgetManager_TextEditor(FPS_Timer* fps_limiter);
	~WidgetManager_TextEditor();

	void Draw() override;
	void Update() override;

	std::vector<std::string> GetNamesTextEditorChild();

	Widget_TextEditor* GetFocusedTextEditor();
	Widget_TextEditor* GetLastFocusedTextEditor();


	std::vector<Widget_TextEditor*> GetWidgets();


	void LoadColors() override;
	std::vector<NamedColor> GetDefaultLightColors() override;
	std::vector<NamedColor> GetDefaultDarkColors() override;


	nlohmann::json Save() override;
	void Load(const nlohmann::json& Data) override;

	void Add(const std::wstring& Path);


	void CloseAll();
	
	void ClearAll();
	void SetActiveFromPath(const std::wstring& Path);

	void SaveCurrentActiveFile();
	void SaveAllFile();

	Widget_TextEditor* GetWidgetFromPath(const std::wstring& Path);

private:
	FPS_Timer* fps_limiter = nullptr;

	std::string LastActiveWidget;

	std::vector<Widget_TextEditor*> widgets;





};



#endif
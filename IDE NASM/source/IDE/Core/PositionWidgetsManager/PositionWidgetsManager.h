#ifndef POSITION_WIDGETS_MANAGER_H
#define POSITION_WIDGETS_MANAGER_H

#include <string>
#include <nlohmann/json.hpp>

#include "IDE/Core/Widget/WidgetManager.h"


class PositionWidgetsManager {
public:
	PositionWidgetsManager(WidgetManager* widgetManager);
	~PositionWidgetsManager();

    void SetupDefaultLayout();
	void SetFirstTextEditor(const std::string& name);

	void DockTextEditorToLastActive(const std::string& name);

	ImGuiID GetMainDockID();
	void SetDockspaceID(const ImGuiID& id);

	std::string MakeSaveData();
	void LoadSaveData(const std::string& SaveData);


private:
	WidgetManager* widgetManager = nullptr;

	ImGuiID dockspace_id;

	ImGuiID dock_main_id;
	ImGuiID dock_id_down;
	ImGuiID dock_id_left;
	ImGuiID dock_id_right;

};


#endif
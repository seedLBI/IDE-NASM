#include "PositionWidgetsManager.h"
#include "IDE/Core/LocalisationManager/LocalisationManager.h"

PositionWidgetsManager::PositionWidgetsManager(WidgetManager* widgetManager) {
	this->widgetManager = widgetManager;
}

PositionWidgetsManager::~PositionWidgetsManager() {

}

void PositionWidgetsManager::SetDockspaceID(const ImGuiID& id) {
	this->dockspace_id = id;
}

void PositionWidgetsManager::SetFirstTextEditor(const std::string& name) {
	ImGui::DockBuilderDockWindow(name.c_str(), dock_main_id);
}

ImGuiID PositionWidgetsManager::GetMainDockID() {
    ImGuiDockNode* root = ImGui::DockBuilderGetNode(dockspace_id);
    ImGuiDockNode* central = root->CentralNode;
    return central->ID;
}

std::string PositionWidgetsManager::MakeSaveData() {
	std::string ini = ImGui::SaveIniSettingsToMemory();
	return ini;
}

void PositionWidgetsManager::LoadSaveData(const std::string& saveData) {
	ImGui::LoadIniSettingsFromMemory(saveData.data(), saveData.size());
}

void PositionWidgetsManager::SetupDefaultLayout() {

	ImGui::DockBuilderRemoveNode(dockspace_id);

	ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);


	dock_main_id = dockspace_id;

	dock_id_down = ImGui::DockBuilderSplitNode(dock_main_id,
		ImGuiDir_Down,
		0.30f,
		nullptr,
		&dock_main_id);

	dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id,
		ImGuiDir_Right,
		0.30f,
		nullptr,
		&dock_main_id);

	dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id,
		ImGuiDir_Left,
		0.20f,
		nullptr,
		&dock_main_id);

	ImGui::DockBuilderDockWindow("###widgetName.filesViewer", dock_id_left);
	ImGui::DockBuilderDockWindow("###widgetName.outputConsole", dock_id_down);

	ImGui::DockBuilderFinish(dockspace_id);

}
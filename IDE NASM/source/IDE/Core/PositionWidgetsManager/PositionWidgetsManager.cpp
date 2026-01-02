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

std::string PositionWidgetsManager::MakeSaveData() {
	ImVec2 vp_size = ImGui::GetMainViewport()->Size;
	std::string ini = ImGui::SaveIniSettingsToMemory();
	std::string custom = "\n[Viewport]\nSize=" + std::to_string(vp_size.x) + "," + std::to_string(vp_size.y) + "\n";
	return ini + custom;
}

void PositionWidgetsManager::LoadSaveData(const std::string& saveData) {
    size_t viewport_pos = saveData.find("[Viewport]");
    std::string ini_data = saveData;
    ImVec2 saved_vp_size = ImVec2(0, 0);
    bool has_custom = (viewport_pos != std::string::npos);

    if (has_custom) {
        ini_data = saveData.substr(0, viewport_pos);
        std::string custom = saveData.substr(viewport_pos);
        size_t size_pos = custom.find("Size=");
        if (size_pos != std::string::npos) {
            size_pos += 5;
            std::string size_str = custom.substr(size_pos);
            size_t comma_pos = size_str.find(',');
            if (comma_pos != std::string::npos) {
                try {
                    saved_vp_size.x = std::stof(size_str.substr(0, comma_pos));
                    saved_vp_size.y = std::stof(size_str.substr(comma_pos + 1));
                }
                catch (...) {
                    saved_vp_size = ImVec2(0, 0);
                }
            }
        }
    }

    ImGui::LoadIniSettingsFromMemory(ini_data.data(), ini_data.size());

    if (has_custom && saved_vp_size.x > 0 && saved_vp_size.y > 0) {
        ImVec2 current_vp_size = ImGui::GetMainViewport()->Size;
        float scale_x = current_vp_size.x / saved_vp_size.x;
        float scale_y = current_vp_size.y / saved_vp_size.y;

        ImGuiContext* ctx = ImGui::GetCurrentContext();
        ImGuiDockContext* dc = &ctx->DockContext;
        for (auto& p : dc->Nodes.Data) {
            ImGuiDockNode* node = (ImGuiDockNode*)p.val_p;
            if (node) {
                node->Pos.x *= scale_x;
                node->Pos.y *= scale_y;
                node->Size.x *= scale_x;
                node->Size.y *= scale_y;
                node->SizeRef.x *= scale_x;
                node->SizeRef.y *= scale_y;
            }
        }

        for (ImGuiWindow* window : ctx->Windows) {
            window->Pos.x *= scale_x;
            window->Pos.y *= scale_y;
            window->Size.x *= scale_x;
            window->Size.y *= scale_y;
            window->SizeFull.x *= scale_x;
            window->SizeFull.y *= scale_y;
        }
    }
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
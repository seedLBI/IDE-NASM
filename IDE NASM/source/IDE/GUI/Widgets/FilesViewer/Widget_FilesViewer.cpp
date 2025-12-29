#include "Widget_FilesViewer.h"
#include "IDE/Core/LocalisationManager/LocalisationManager.h"

Widget_FilesViewer::Widget_FilesViewer(WidgetManager_TextEditor* widgetManager_TextEditor) : 
    IWidget("widgetName.filesViewer") {
    this->widgetManager_TextEditor = widgetManager_TextEditor;
}

Widget_FilesViewer::~Widget_FilesViewer() {

}

void Widget_FilesViewer::LoadSolutionInfo(SolutionInfo* solutionData_ptr) {
	currentSolution = solutionData_ptr;
    currentSolution->UpdateAllFiles();

    widgetManager_TextEditor->ClearAll();
    for (int i = 0; i < currentSolution->files.size(); i++) {
        widgetManager_TextEditor->Add(currentSolution->GetPathAbsolute() + L"\\" + stringUTF8_to_wstring(currentSolution->files[i]));
    }

	UpdateDirectory();
}

void Widget_FilesViewer::UpdateDirectory() {
	directory_info.clear();
    directory_info = build_directory_json(currentSolution->GetPathAbsolute() + L"\\" + currentSolution->GetPathSource());
    std::cout << directory_info.dump(4) << std::endl;
}

nlohmann::json Widget_FilesViewer::build_directory_json(const std::wstring& path) {
    std::filesystem::path dir_path(path);
    if (!std::filesystem::exists(dir_path) || !std::filesystem::is_directory(dir_path)) {
        return {};
    }

    nlohmann::json root = {
        {"name", currentSolution->name},
        {"type", "directory"},
        {"children", nlohmann::json::array()}
    };


    std::function<void(const std::filesystem::path&, nlohmann::json&)> build_json;
    build_json = [&build_json](const std::filesystem::path& current_path, nlohmann::json& current_node) {
        for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
            nlohmann::json new_node = {
                {"name", entry.path().filename().u8string()},
                {"type", entry.is_directory() ? "directory" : "file"},
                {"fullpath", entry.path().u8string()}
            };

            if (entry.is_directory()) {
                new_node["children"] = nlohmann::json::array();
                build_json(entry.path(), new_node);
            }

            current_node["children"].push_back(new_node);
        }
    };

    build_json(dir_path, root);
    return root;
}
void Widget_FilesViewer::render_imgui_tree(const nlohmann::json& node) {
    if (node["type"] == "directory") {

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.1f , 0.5f, 1.f));


        const std::string STR_BUTTON_CREATE_FILE = ICON_FA_FILE_CIRCLE_PLUS;
        const std::string STR_BUTTON_CREATE_FOLDER = ICON_FA_FOLDER_PLUS;
        const std::string STR_BUTTON_DELETE_FOLDER = ICON_FA_FOLDER_MINUS;

        const std::string nameFolder = node["name"].get<std::string>();

        auto pos_find_folder = std::find(folders_open.begin(), folders_open.end(), nameFolder);
        bool isOpen = pos_find_folder != folders_open.end();
        std::string label = (isOpen ? ICON_FA_FOLDER_OPEN : ICON_FA_FOLDER) + std::string(" ") + node["name"].get<std::string>() + "##FILESVIEWER";


        ImGuiID ID_Tree = ImGui::GetID(nameFolder.c_str());
        const ImGuiTreeNodeFlags flags = 
            ImGuiTreeNodeFlags_DefaultOpen | 
            ImGuiTreeNodeFlags_OpenOnArrow | 
            ImGuiTreeNodeFlags_OpenOnDoubleClick | 
            ImGuiTreeNodeFlags_SpanFullWidth | 
            ImGuiTreeNodeFlags_AllowItemOverlap;


        bool is_open = ImGui::TreeNodeBehavior(ID_Tree, flags, label.c_str());
        ImGui::SetItemAllowOverlap();



        // After drawing TreeNode, get its full rect
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiContext& g = *GImGui;
        float lineHeight = ImGui::GetFrameHeight();
        float btnWidth = lineHeight;
        //float spacing = g.Style.ItemInnerSpacing.x;
        float spacing = 0.f;
        ImVec2 itemMin = ImGui::GetItemRectMin();
        ImVec2 itemMax = ImGui::GetItemRectMax() ;
        ImRect btnRgn(itemMax.x - btnWidth * 2 - spacing, itemMin.y, itemMax.x, itemMin.y + btnWidth);

        // Handle button input and consume click to prevent TreeNode toggle
        ImGuiIO& io = ImGui::GetIO();
        // Create Folder button
        ImGuiID btnId1 = ImGui::GetID((nameFolder + "_btn_folder").c_str());
        bool hovered1, held1;

        

        if (ImGui::ButtonBehavior(ImRect(btnRgn.Min, ImVec2(btnRgn.Min.x + btnWidth, btnRgn.Max.y - 2.f)), btnId1, &hovered1, &held1, 0)) {
            ImGui::OpenPopup("create_folder_popup");
        }
        // Create File button
        ImGuiID btnId2 = ImGui::GetID((nameFolder + "_btn_file").c_str());
        bool hovered2, held2;
        if (ImGui::ButtonBehavior(ImRect(ImVec2(btnRgn.Min.x + btnWidth + spacing, btnRgn.Min.y - 2.f), btnRgn.Max), btnId2, &hovered2, &held2, 0)) {
            ImGui::OpenPopup("create_file_popup");
        }
        // Consume mouse click if on any button
        if ((hovered1 || hovered2) && io.MouseClicked[0])
            io.MouseClicked[0] = false;

        // Draw "+" texts for buttons
        ImU32 textColor1 = hovered1 ? ImGui::GetColorU32(ImGuiCol_Text) : ImGui::GetColorU32(ImGuiCol_TextDisabled);
        window->DrawList->AddText(ImVec2(btnRgn.Min.x + btnWidth * 0.25f, btnRgn.Min.y), textColor1, STR_BUTTON_CREATE_FILE.c_str());
        ImU32 textColor2 = hovered2 ? ImGui::GetColorU32(ImGuiCol_Text) : ImGui::GetColorU32(ImGuiCol_TextDisabled);
        window->DrawList->AddText(ImVec2(btnRgn.Min.x + btnWidth + spacing + btnWidth * 0.25f, btnRgn.Min.y), textColor2, STR_BUTTON_CREATE_FOLDER.c_str());


        if (is_open) {
            if (!isOpen) {
                folders_open.push_back(nameFolder);
            }
            ImGui::PopStyleColor();
            if (node.contains("children")) {
                for (const auto& child : node["children"]) {
                    render_imgui_tree(child);
                }
            }
            ImGui::TreePop();
        }
        else {
            if (pos_find_folder != folders_open.end()) {
                folders_open.erase(pos_find_folder);
            }
            ImGui::PopStyleColor();
        }
    }
    else if (node["type"] == "file") {

        bool IsMain = node["fullpath"].get<std::string>() == wstring_to_stringUTF8(currentSolution->GetPathAbsolute() + L"\\") + currentSolution->mainFile;
        bool IsActive = false;

        Widget_TextEditor* ptr_widget = widgetManager_TextEditor->GetFocusedTextEditor();
        if (ptr_widget != nullptr)
            IsActive = node["fullpath"].get<std::string>() == wstring_to_stringUTF8(ptr_widget->GetFilePath());


        if (IsMain) {
            ImGui::Text(ICON_FA_CIRCLE_DOT);
            TooltipTranslated("filesViewer.entryPoint.tooltip");
        }
        else {
            ImGui::Dummy(ImVec2(ImGui::CalcTextSize(ICON_FA_CIRCLE_DOT).x, 0));
        }
        ImGui::SameLine();



        //std::cout << widgetManager_TextEditor->GetFocusedTextEditor() << std::endl;


        if (!IsActive) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
        }

        if (ImGui::Selectable(std::string( node["name"].get<std::string>()).c_str())) {

            widgetManager_TextEditor->SetActiveFromPath(stringUTF8_to_wstring(node["fullpath"].get<std::string>()));
        }
        TooltipTranslated("filesViewer.fileActive.tooltip");


        if (!IsActive) {
            ImGui::PopStyleColor();
        }

        //ImGui::Text("%s", node["name"].get<std::string>().c_str());
    }
}



void Widget_FilesViewer::Draw() {
	if (GetFlagShow() == false)
		return;

	if (ImGui::Begin(tr(GetName_c_str()).c_str(), GetPtrFlagShow())) {

        render_imgui_tree(directory_info);

        ImGui::End();
	}
    else {
        ImGui::End();
    }

}

void Widget_FilesViewer::Update() {
	UpdateActive();
}


nlohmann::json Widget_FilesViewer::Save() {

    nlohmann::json result;

    result["flag_Show"] = GetFlagShow();

    return result;
}

void Widget_FilesViewer::Load(const nlohmann::json& Data) {
    if (Data.contains("flag_Show"))
        SetFlagShow(Data["flag_Show"].get<bool>());
}
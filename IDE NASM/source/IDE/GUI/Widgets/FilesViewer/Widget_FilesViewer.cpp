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
        {"fullpath", dir_path.u8string()},
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

        auto comparator = [](const nlohmann::json& a, const nlohmann::json& b) {
            std::string type_a = a["type"];
            std::string type_b = b["type"];
            bool a_dir = (type_a == "directory");
            bool b_dir = (type_b == "directory");
            if (a_dir != b_dir) {
                return a_dir;
            }
            else {
                std::string name_a = a["name"];
                std::string name_b = b["name"];
                return name_a < name_b;
            }
            };
        std::sort(current_node["children"].begin(), current_node["children"].end(), comparator);
    };

    build_json(dir_path, root);
    return root;
}



void Widget_FilesViewer::render_imgui_tree(const nlohmann::json& node) {

    static std::string selected_item_context_menu = "";

    std::string s_open            = (ICON_FA_FOLDER_OPEN + std::string(" ") + tr("filesViewer.contextMenu.Open"));
    std::string s_cut             = (ICON_FA_SCISSORS + std::string(" ") + tr("filesViewer.contextMenu.Cut"));
    std::string s_copy            = (ICON_FA_COPY + std::string(" ") + tr("filesViewer.contextMenu.Copy"));
    std::string s_paste           = (ICON_FA_PASTE + std::string(" ") + tr("filesViewer.contextMenu.Paste"));
    std::string s_rename          = (ICON_FA_PENCIL + std::string(" ") + tr("filesViewer.contextMenu.Rename"));
    std::string s_delete          = (ICON_FA_TRASH + std::string(" ") + tr("filesViewer.contextMenu.Delete"));
    std::string s_createFile      = (ICON_FA_FILE_CIRCLE_PLUS + std::string(" ") + tr("filesViewer.contextMenu.CreateFile"));
    std::string s_createFolder    = (ICON_FA_FOLDER_PLUS + std::string(" ") + tr("filesViewer.contextMenu.CreateFolder"));
    std::string s_add             = (ICON_FA_CIRCLE_PLUS + std::string(" ") + tr("filesViewer.contextMenu.Add"));
    std::string s_openInExplorer  = (ICON_FA_FOLDER + std::string(" ") + tr("filesViewer.contextMenu.OpenInExplorer"));
    std::string s_setAsEntryPoint = (ICON_FA_CIRCLE_DOT + std::string(" ") + tr("filesViewer.contextMenu.SetEntryPoint"));

    if (node["type"] == "directory") {

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

        

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.1f, 0.5f, 1.f));
        bool is_open = ImGui::TreeNodeBehavior(ID_Tree, flags, label.c_str());
        ImGui::PopStyleColor();

        if (ImGui::BeginPopupContextItem())
        {



            if (ImGui::BeginMenu(s_add.c_str()))
            {
                if (ImGui::MenuItem(s_createFile.c_str())) {
                    
                }
                if (ImGui::MenuItem(s_createFolder.c_str())) {

                }
                ImGui::EndMenu();
            }

            ImGui::Separator();

            ImGui::Selectable(s_cut.c_str());
            ImGui::Selectable(s_copy.c_str());

            ImGui::BeginDisabled();
                ImGui::Selectable(s_paste.c_str());
            ImGui::EndDisabled();

            ImGui::Selectable(s_rename.c_str());
            ImGui::Selectable(s_delete.c_str());

            ImGui::Separator();

            if (ImGui::Selectable(s_openInExplorer.c_str())) {
                OpenInExplorer(node["fullpath"].get<std::string>());
            }


            ImGui::EndPopup();
        }


        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.1f, 0.5f, 1.f));


        ImGui::SetItemAllowOverlap();

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

        std::string name = node["name"].get<std::string>();
        std::string full_path = node["fullpath"].get<std::string>();
        std::string main_path = wstring_to_stringUTF8(currentSolution->GetPathAbsolute() + L"\\") + currentSolution->mainFile;



        //std::string ralative_path = wstring_to_stringUTF8(currentSolution->GetPathAbsolute() + L"\\") + currentSolution->mainFile;

        bool IsMain = full_path == main_path;
        bool IsActive = false;
        bool IsOpen = false;


        Widget_TextEditor* current_widget = widgetManager_TextEditor->GetWidgetFromPath(
            stringUTF8_to_wstring(full_path)
        );
        if (current_widget != nullptr) {
            IsOpen = current_widget->GetFlagShow();
        }

        Widget_TextEditor* ptr_widget = widgetManager_TextEditor->GetFocusedTextEditor();
        if (ptr_widget != nullptr)
            IsActive = full_path == wstring_to_stringUTF8(ptr_widget->GetFilePath());



        

        if (IsMain) {
            ImGui::Text(ICON_FA_CIRCLE_DOT);
            TooltipTranslated("filesViewer.entryPoint.tooltip");
        }
        else {
            ImGui::Dummy(ImVec2(ImGui::CalcTextSize(ICON_FA_CIRCLE_DOT).x, 0));
        }
        ImGui::SameLine();



        if (!IsOpen) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
        }

        if (IsActive)
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);


        if (ImGui::Selectable(name.c_str(), selected_item_context_menu == full_path)) {

            widgetManager_TextEditor->SetActiveFromPath(stringUTF8_to_wstring(full_path));
        }



        if (IsActive)
            ImGui::PopFont();

        if (!IsOpen) {
            ImGui::PopStyleColor();
        }

        selected_item_context_menu = "";
        if (ImGui::BeginPopupContextItem())
        {

            std::cout << "main: " << currentSolution->mainFile << std::endl;
            std::cout << "name: " << name << std::endl;
            std::cout << "full: " << full_path << std::endl;


            selected_item_context_menu = full_path;

            if (ImGui::Selectable(s_open.c_str())) {
                widgetManager_TextEditor->SetActiveFromPath(stringUTF8_to_wstring(full_path));
            }
            ImGui::Separator();

            if (ImGui::Selectable(s_setAsEntryPoint.c_str())) {
                //currentSolution->entryPoint = 
            }

            ImGui::Separator();
            ImGui::Selectable(s_cut.c_str());
            ImGui::Selectable(s_copy.c_str());
            ImGui::Selectable(s_rename.c_str());
            ImGui::Selectable(s_delete.c_str());
            ImGui::EndPopup();
        }





        if(IsActive)
            TooltipTranslated("filesViewer.fileActive.tooltip");
        else if (!IsActive && IsOpen)
            TooltipTranslated("filesViewer.fileNotActive.tooltip");
        else if (!IsOpen)
            TooltipTranslated("filesViewer.fileNotOpen.tooltip");

    }
}



void Widget_FilesViewer::Draw() {
	if (GetFlagShow() == false)
		return;

	if (ImGui::Begin(IWidget::GetNameTranslated().c_str(), GetPtrFlagShow())) {

        render_imgui_tree(directory_info);

        ImGui::End();
	}
    else {
        ImGui::End();
    }

}

void Widget_FilesViewer::OpenInExplorer(const std::string& path) {
    std::string command = "explorer /select,\"" + path + "\"";
    system(command.c_str());
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
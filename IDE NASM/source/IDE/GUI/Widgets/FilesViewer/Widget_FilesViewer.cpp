#include "Widget_FilesViewer.h"
#include "IDE/Core/LocalisationManager/LocalisationManager.h"

Widget_FilesViewer::Widget_FilesViewer(WidgetManager_TextEditor* widgetManager_TextEditor) : 
    IWidget("widgetName.filesViewer"),
    IThemeLoadable("themeItem.filesViewer") {
    this->widgetManager_TextEditor = widgetManager_TextEditor;

    IThemeLoadable::InitListWord(
        {
            "color.filesViewer.folder.open",
            "color.filesViewer.folder.close",
            "color.filesViewer.background.project",
            "color.filesViewer.text.project",
        }
   );

}

Widget_FilesViewer::~Widget_FilesViewer() {

}


void Widget_FilesViewer::LoadColors() {
    std::string toSearch;

    static robin_hood::unordered_flat_map<std::string, ImColor*> translator = {
        {"color.filesViewer.folder.open",    &color_folder_open},
        {"color.filesViewer.folder.close",   &color_folder_close},
        {"color.filesViewer.background.project",   &color_background_Project},
        {"color.filesViewer.text.project",   &color_text_Project},

    };


    for (int i = 0; i < object_colors.colors.size(); i++) {

        toSearch = object_colors.colors[i].nameColor;

        if (translator.contains(toSearch))
            *translator[toSearch] = object_colors.colors[i].color;
    }
}
std::vector<NamedColor> Widget_FilesViewer::GetDefaultLightColors() {

    /*
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f - 0.12f, 1.f - 0.12f, 1.f - 0.12f, 1.0f));
    */

    std::vector<NamedColor> colors = {
        {"color.filesViewer.folder.open",	        ImColor(188, 139, 0, 255)},
        {"color.filesViewer.folder.close",	        ImColor(145, 109, 6, 255)},
        {"color.filesViewer.background.project",	ImColor(0.12f, 0.12f, 0.12f, 1.0f)},
        {"color.filesViewer.text.project",	        ImColor(1.f - 0.12f, 1.f - 0.12f, 1.f - 0.12f, 1.0f)},
    };

    return colors;

}
std::vector<NamedColor> Widget_FilesViewer::GetDefaultDarkColors() {
    std::vector<NamedColor> colors = {
        {"color.filesViewer.folder.open",	        ImColor(255,141,0, 255)},
        {"color.filesViewer.folder.close",	        ImColor(136,76,0,255)},
        {"color.filesViewer.background.project",	ImColor(1.f - 0.12f, 1.f - 0.12f, 1.f - 0.12f, 1.0f)},
        {"color.filesViewer.text.project",	        ImColor(0.12f, 0.12f, 0.12f, 1.0f)},
    };

    return colors;
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



void Widget_FilesViewer::Update() {
    UpdateActive();
    UpdateWords();
    ProcessState();
}
void Widget_FilesViewer::ProcessState() {

    switch (current_state) {
    case StateOperation::CreationFile:
        _CreateFile(forstate_directory, forstate_name);
        break;
    case StateOperation::CreationFolder:
        _CreateFolder(forstate_directory, forstate_name);
        break;
    case StateOperation::DeletePath:
        _DeletePath(forstate_full_path);
        break;
    case StateOperation::RenamePath:
        _RenamePath(forstate_full_path, forstate_name);
        break;
    case StateOperation::PastePath:
        _PastePath(forstate_directory);
        break;
    case StateOperation::Nothing:
    default:
        break;
    }

    current_state = StateOperation::Nothing;
}
void Widget_FilesViewer::UpdateWords() {
    s_open = (ICON_FA_FOLDER_OPEN + std::string(" ") + tr("filesViewer.contextMenu.Open"));
    s_cut = (ICON_FA_SCISSORS + std::string(" ") + tr("filesViewer.contextMenu.Cut"));
    s_copy = (ICON_FA_COPY + std::string(" ") + tr("filesViewer.contextMenu.Copy"));
    s_paste = (ICON_FA_PASTE + std::string(" ") + tr("filesViewer.contextMenu.Paste"));
    s_rename = (ICON_FA_PENCIL + std::string(" ") + tr("filesViewer.contextMenu.Rename"));
    s_delete = (ICON_FA_TRASH + std::string(" ") + tr("filesViewer.contextMenu.Delete"));
    s_createFile = (ICON_FA_FILE_CIRCLE_PLUS + std::string(" ") + tr("filesViewer.contextMenu.CreateFile"));
    s_createFolder = (ICON_FA_FOLDER_PLUS + std::string(" ") + tr("filesViewer.contextMenu.CreateFolder"));
    s_add = (ICON_FA_CIRCLE_PLUS + std::string(" ") + tr("filesViewer.contextMenu.Add"));
    s_openInExplorer = (ICON_FA_FOLDER + std::string(" ") + tr("filesViewer.contextMenu.OpenInExplorer"));
    s_setAsEntryPoint = (ICON_FA_CIRCLE_DOT + std::string(" ") + tr("filesViewer.contextMenu.SetEntryPoint"));
}
void Widget_FilesViewer::UpdateDirectory() {
	directory_info.clear();

    directory_info = build_directory_json(currentSolution->GetPathAbsolute() + L"\\" + currentSolution->GetPathSource());
    std::cout << directory_info.dump(4) << std::endl;
}



static int FileNameInputCallback(ImGuiInputTextCallbackData* data)
{
    const char* invalid_chars = "<>:\"/\\|?*.";

    if (data->EventFlag == ImGuiInputTextFlags_CallbackCharFilter)
    {
        if (data->EventChar < 32)
            return 1;

        for (const char* c = invalid_chars; *c != '\0'; c++) {
            if (data->EventChar == *c)
                return 1;
        }

        if (data->EventChar == '.' && data->CursorPos == 0)
            return 1;

        if (data->EventChar == '.') {
            if (data->CursorPos > 0 && data->Buf[data->CursorPos - 1] == '.')
                return 1;
        }
    }

    return 0;
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
    if (node.size() == 0)
        return;


    if (node["type"] == "directory") {

        const std::string nameFolder = node["name"].get<std::string>();
        const std::string fullpathFolder = node["fullpath"].get<std::string>();

        auto pos_find_folder = std::find(folders_open.begin(), folders_open.end(), fullpathFolder);
        bool isOpen = pos_find_folder != folders_open.end();

        //bool isFolderProject = fullpathFolder == wstring_to_stringUTF8(currentSolution->GetPathAbsolute() + L"\\" + currentSolution->GetPathSource());
        bool is_renaming = (path_to_rename == fullpathFolder);


        std::string label_type_folder = (isOpen ? ICON_FA_FOLDER_OPEN : ICON_FA_FOLDER);
        std::string label_name = node["name"].get<std::string>();
        std::string label_full = label_type_folder + label_name;

        ImGuiID ID_Tree = ImGui::GetID((nameFolder + "###" + node["fullpath"].get<std::string>()).c_str());
        const ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_SpanFullWidth |
            ImGuiTreeNodeFlags_AllowItemOverlap;

        bool is_open = true;

        is_open = ImGui::TreeNodeBehavior(ID_Tree, flags, "");

        DrawContextMenu_Folder(fullpathFolder);

        float spacing = ImGui::GetTreeNodeToLabelSpacing();
        ImGui::SameLine(0.f, spacing * 0.5f);

        ImColor colorFolder = isOpen ? color_folder_open : color_folder_close;
        ImGui::TextColored(colorFolder, "%s", label_type_folder.c_str());


        


        if (is_renaming) {

            ImGui::SameLine(0.f, 5.f);

            if (need_focus_rename) {
                ImGui::SetKeyboardFocusHere();
                need_focus_rename = false;
            }
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 0.5f));
                ImGui::InputText("###RENAMING_INPUT_TEXT_FOLDER", rename_buffer, 256, ImGuiInputTextFlags_CallbackCharFilter, FileNameInputCallback);
            ImGui::PopStyleVar();

            if (ImGui::IsItemDeactivated()) {
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    path_to_rename = "";
                    SetState_RenamePath(fullpathFolder);
                }
                else {
                    path_to_rename = "";
                }
            }
        }
        else{

            ImGui::SameLine(0.f, isOpen ? 0.0f : 2.f);
            ImGui::Text(" %s", label_name.c_str());
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.1f, 0.5f, 1.f));


        ImGui::SetItemAllowOverlap();

        
        if (is_open) {
            if (!isOpen) {
                folders_open.push_back(fullpathFolder);
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
        bool is_renaming = (path_to_rename == full_path);
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

        if (is_renaming) {

            ImGui::SameLine(0.f, 5.f);

            if (need_focus_rename) {
                ImGui::SetKeyboardFocusHere();
                need_focus_rename = false;
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 0.5f));
                ImGui::InputText("###RENAMING_INPUT_TEXT_FILE", rename_buffer, 256, ImGuiInputTextFlags_CallbackCharFilter, FileNameInputCallback);
            ImGui::PopStyleVar();

            if (ImGui::IsItemDeactivated()) {
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    path_to_rename = "";
                    SetState_RenamePath(full_path);
                }
                else {
                    path_to_rename = "";
                }
            }
        }
        else {

            ImGui::SameLine();



            if (!IsOpen) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            }

            if (IsActive)
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);


            if (ImGui::Selectable(name.c_str())) {

                widgetManager_TextEditor->SetActiveFromPath(stringUTF8_to_wstring(full_path));
            }



            if (IsActive)
                ImGui::PopFont();

            if (!IsOpen) {
                ImGui::PopStyleColor();
            }
        }

       





        DrawContextMenu_File(full_path);





        if(IsActive)
            TooltipTranslated("filesViewer.fileActive.tooltip");
        else if (!IsActive && IsOpen)
            TooltipTranslated("filesViewer.fileNotActive.tooltip");
        else if (!IsOpen)
            TooltipTranslated("filesViewer.fileNotOpen.tooltip");

    }
}


bool Widget_FilesViewer::_CreateFile(const std::string& directory, const std::string& nameFile) {
    std::filesystem::path file_path(stringUTF8_to_wstring(directory + "\\" + nameFile + ".asm"));

    if (std::filesystem::exists(file_path))
        return false;

    std::ofstream file(file_path);
    if (!file.is_open())
        return false;
    
    file.close();

    UpdateDirectory();
    currentSolution->UpdateAllFiles();
    widgetManager_TextEditor->Add(file_path.wstring());
    widgetManager_TextEditor->SetActiveFromPath(file_path.wstring());

    return true;
}
bool Widget_FilesViewer::_CreateFolder(const std::string& directory, const std::string& nameFolder) {

    std::filesystem::path folder_path(stringUTF8_to_wstring( directory + "\\" + nameFolder));

    if (std::filesystem::exists(folder_path))
        return false;


    std::filesystem::create_directory(folder_path);
    UpdateDirectory();

    return true;
}
bool Widget_FilesViewer::_DeletePath(const std::string& full_path) {

    std::wstring path = stringUTF8_to_wstring(full_path);

    if (std::filesystem::exists(path)) {

        if (std::filesystem::is_regular_file(path))
            widgetManager_TextEditor->Remove(path);

        std::filesystem::remove_all(path);
        UpdateDirectory();
        currentSolution->UpdateAllFiles();

        

        return true;
    }
    return false;
}
bool Widget_FilesViewer::_RenamePath(const std::string& old_full_path, const std::string& newName) {
    std::filesystem::path old_file_path = stringUTF8_to_wstring(old_full_path);
    std::filesystem::path new_file_path = old_file_path.parent_path() / stringUTF8_to_wstring(newName);

    try {
        if (std::filesystem::exists(old_file_path) && !std::filesystem::exists(new_file_path)) {

            if (std::filesystem::is_regular_file(old_file_path)) {
                new_file_path += ".asm";
            }

            std::filesystem::rename(old_file_path, new_file_path);
            UpdateDirectory();
            currentSolution->UpdateAllFiles();

            // Обновляем путь в редакторе если файл был открыт
            Widget_TextEditor* widget = widgetManager_TextEditor->GetWidgetFromPath(
                stringUTF8_to_wstring(old_full_path)
            );
            if (widget != nullptr) {
                widgetManager_TextEditor->Remove(stringUTF8_to_wstring(old_full_path));
                widgetManager_TextEditor->Add(stringUTF8_to_wstring(new_file_path.u8string()));
            }
        }
    }
    catch (...) {
        return false;
    }
    return true;

}
void Widget_FilesViewer::_OpenPopupCreationFile(const std::string& full_path) {
    show_create_file_popup = true;
    need_focus_popup = true;
    selected_context_full_path = full_path;
}
void Widget_FilesViewer::_OpenPopupCreationFolder(const std::string& full_path) {
    show_create_folder_popup = true;
    need_focus_popup = true;
    selected_context_full_path = full_path;
}
void Widget_FilesViewer::_SetRenameMode(const std::string& full_path) {
    path_to_rename = full_path;
    memset(rename_buffer, 0, 256);

    std::string name = std::filesystem::path(stringUTF8_to_wstring(full_path)).stem().u8string();

    strcpy_s(rename_buffer, 256, name.c_str());
    need_focus_rename = true;
}
void Widget_FilesViewer::_CopyPath(const std::string& full_path) {
    clipboard.path = full_path;
    clipboard.is_cut = false;
    clipboard.is_valid = true;
}
void Widget_FilesViewer::_CutPath(const std::string& full_path) {
    clipboard.path = full_path;
    clipboard.is_cut = true;
    clipboard.is_valid = true;
}
bool Widget_FilesViewer::_PastePath(const std::string& destination_folder) {
    if (!clipboard.is_valid || clipboard.path.empty())
        return false;

    std::filesystem::path source_path = stringUTF8_to_wstring(clipboard.path);
    std::filesystem::path dest_folder = stringUTF8_to_wstring(destination_folder);

    if (source_path == dest_folder / source_path.filename()) {
        return false;
    }

    if (!std::filesystem::exists(source_path))
        return false;

    std::filesystem::path dest_path = dest_folder / source_path.filename();

    if (std::filesystem::exists(dest_path)) {
        std::wstring base_name = dest_path.stem().wstring();
        std::wstring extension = dest_path.extension().wstring();
        int counter = 1;

        do {
            std::wstring new_name = base_name + L"_" + std::to_wstring(counter) + extension;
            dest_path = dest_folder / new_name;
            counter++;
        } while (std::filesystem::exists(dest_path));
    }

    try {
        if (clipboard.is_cut) {
            std::filesystem::rename(source_path, dest_path);

            if (std::filesystem::is_regular_file(dest_path)) {
                Widget_TextEditor* widget = widgetManager_TextEditor->GetWidgetFromPath(
                    stringUTF8_to_wstring(clipboard.path)
                );
                if (widget != nullptr) {
                    widgetManager_TextEditor->Remove(stringUTF8_to_wstring(clipboard.path));
                    widgetManager_TextEditor->Add(dest_path.wstring());
                }
            }

            clipboard.is_valid = false; 
        }
        else {
            if (std::filesystem::is_directory(source_path)) {
                std::filesystem::copy(source_path, dest_path,
                    std::filesystem::copy_options::recursive);
            }
            else {
                std::filesystem::copy_file(source_path, dest_path);
            }
        }

        UpdateDirectory();
        currentSolution->UpdateAllFiles();

        if (std::filesystem::is_regular_file(dest_path)) {
            widgetManager_TextEditor->Add(dest_path.wstring());
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Paste error: " << e.what() << std::endl;
        return false;
    }
}
bool Widget_FilesViewer::_SetAsEntryPoint(const std::string& full_path) {
    std::filesystem::path file_path = stringUTF8_to_wstring(full_path);
    std::filesystem::path solution_base = currentSolution->GetPathAbsolute() + L"\\";

    std::filesystem::path relative = std::filesystem::relative(file_path, solution_base);

    currentSolution->mainFile = wstring_to_stringUTF8(relative.wstring());

    return true;
}


void Widget_FilesViewer::SetState_CreationFile(const std::string& directory, const std::string& nameFile) {
    forstate_name = nameFile;
    forstate_directory = directory;
    current_state = StateOperation::CreationFile;
}
void Widget_FilesViewer::SetState_CreationFolder(const std::string& directory, const std::string& nameFolder) {
    forstate_name = nameFolder;
    forstate_directory = directory;
    current_state = StateOperation::CreationFolder;
}
void Widget_FilesViewer::SetState_DeletePath(const std::string& full_path) {
    forstate_full_path = full_path;
    current_state = StateOperation::DeletePath;
}
void Widget_FilesViewer::SetState_RenamePath(const std::string& full_path) {
    forstate_full_path = full_path;
    forstate_name = rename_buffer;
    current_state = StateOperation::RenamePath;
}
void Widget_FilesViewer::SetState_PastePath(const std::string& destination_folder) {
    forstate_directory = destination_folder;
    current_state = StateOperation::PastePath;
}


void Widget_FilesViewer::DrawContextMenu_File(const std::string& fullpath) {
    if (ImGui::BeginPopupContextItem())
    {

        if (ImGui::Selectable(s_open.c_str())) {
            widgetManager_TextEditor->SetActiveFromPath(stringUTF8_to_wstring(fullpath));
        }
        ImGui::Separator();

        if (ImGui::Selectable(s_setAsEntryPoint.c_str())) {
            _SetAsEntryPoint(fullpath);
        }

        ImGui::Separator();
        if (ImGui::Selectable(s_cut.c_str())) {
            _CutPath(fullpath);
        }
        if (ImGui::Selectable(s_copy.c_str())) {
            _CopyPath(fullpath);
        }
        if (ImGui::Selectable(s_rename.c_str())) {
            _SetRenameMode(fullpath);
        }
        if (ImGui::Selectable(s_delete.c_str())) {
            SetState_DeletePath(fullpath);
        }
        ImGui::EndPopup();
    }
}
void Widget_FilesViewer::DrawContextMenu_Folder(const std::string& fullpath) {
    if (ImGui::BeginPopupContextItem()) {

        if (ImGui::BeginMenu(s_add.c_str()))
        {
            if (ImGui::MenuItem(s_createFile.c_str())) {
                _OpenPopupCreationFile(fullpath);
            }
            if (ImGui::MenuItem(s_createFolder.c_str())) {
                _OpenPopupCreationFolder(fullpath);
            }
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::Selectable(s_cut.c_str())) {
            _CutPath(fullpath);
        }
        if (ImGui::Selectable(s_copy.c_str())) {
            _CopyPath(fullpath);
        }
        ImGui::BeginDisabled(!clipboard.is_valid);
            if (ImGui::Selectable(s_paste.c_str())) {
                SetState_PastePath(fullpath);
            }
        ImGui::EndDisabled();


        if(ImGui::Selectable(s_rename.c_str())){
            _SetRenameMode(fullpath);
        }
        if (ImGui::Selectable(s_delete.c_str())) {
            SetState_DeletePath(fullpath);
        }

        ImGui::Separator();

        if (ImGui::Selectable(s_openInExplorer.c_str())) {
            _OpenInExplorer(fullpath);
        }


        ImGui::EndPopup();
    }
}
void Widget_FilesViewer::DrawPopup_CreationFolder() {

    if (show_create_folder_popup) {
        ImGui::OpenPopup("###POPUP_CREATION_FOLDER_FilesViewer");
        show_create_folder_popup = false;

        memset(create_buffer, 0, 256);
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal((tr("filesViewer.popup.folder.title") + "###POPUP_CREATION_FOLDER_FilesViewer").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {



        TextCenteredOnLine(tr("filesViewer.popup.folder.text").c_str(), 0, 0);
        



        if (need_focus_popup) {
            ImGui::SetKeyboardFocusHere();
            need_focus_popup = false;
            
        }

        if (ImGui::InputText(
            "###create_folder_input",
            create_buffer,
            sizeof(create_buffer),
            ImGuiInputTextFlags_EnterReturnsTrue
        )) {
            if (strlen(create_buffer) > 0) {
                SetState_CreationFolder(selected_context_full_path, create_buffer);
            }

            ImGui::CloseCurrentPopup();
        }
        ImGui::Dummy(ImVec2(0.f, 2.f));

        if (ButtonCenteredOnLine(tr("filesViewer.popup.folder.ok").c_str(), 0.5f)) {
            if (strlen(create_buffer) > 0) {
                SetState_CreationFolder(selected_context_full_path, create_buffer);
            }

            ImGui::CloseCurrentPopup();
        }


        if (ButtonCenteredOnLine(tr("filesViewer.popup.folder.cancel").c_str(), 0.5f) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(1);

}
void Widget_FilesViewer::DrawPopup_CreationFile() {
    if (show_create_file_popup) {
        ImGui::OpenPopup("###POPUP_CREATION_FILE_FilesViewer");
        show_create_file_popup = false;

        memset(create_buffer, 0, 256);
    }


    if (ImGui::BeginPopupModal((tr("filesViewer.popup.file.title") + "###POPUP_CREATION_FILE_FilesViewer").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

        TextCenteredOnLine(tr("filesViewer.popup.file.text").c_str(), 0, 0);


        if (need_focus_popup)
        {
            ImGui::SetKeyboardFocusHere();
            need_focus_popup = false;
        }

        if (ImGui::InputText(
            "##create_file_input",
            create_buffer,
            sizeof(create_buffer),
            ImGuiInputTextFlags_EnterReturnsTrue
        )) {
            if (strlen(create_buffer) > 0) {
                SetState_CreationFile(selected_context_full_path, create_buffer);
            }

            ImGui::CloseCurrentPopup();
        }


        ImGui::Dummy(ImVec2(0.f, 2.f));

        if (ButtonCenteredOnLine(tr("filesViewer.popup.file.ok").c_str(), 0.5f)) {
            if (strlen(create_buffer) > 0) {
                SetState_CreationFile(selected_context_full_path, create_buffer);
            }

            ImGui::CloseCurrentPopup();
        }


        if (ButtonCenteredOnLine(tr("filesViewer.popup.file.cancel").c_str(), 0.5f) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}





void Widget_FilesViewer::Draw() {
	if (GetFlagShow() == false)
		return;

    DrawPopup_CreationFolder();
    DrawPopup_CreationFile();

    std::string current_solution_path = wstring_to_stringUTF8(currentSolution->GetPathAbsolute() + L"\\" + currentSolution->GetPathSource());

	if (ImGui::Begin(IWidget::GetNameTranslated().c_str(), GetPtrFlagShow())) {


        ImGui::PushStyleColor(ImGuiCol_ChildBg, color_background_Project.Value);
        ImGui::PushStyleColor(ImGuiCol_Text, color_text_Project.Value);
            ImGui::BeginChild("ProjectNameBg", ImVec2(0, ImGui::GetTextLineHeightWithSpacing()), false);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f); // Горизонтальный отступ
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);  // Вертикальный отступ
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
                    ImGui::Text((tr("filesViewer.project") + " \"" + currentSolution->name + "\"").c_str());
                ImGui::PopFont();
            ImGui::EndChild();
        ImGui::PopStyleColor(2);

        for (const auto& child : directory_info["children"]) {
            render_imgui_tree(child);
        }

        ImGui::InvisibleButton("##empty_space", ImGui::GetContentRegionAvail());
        DrawContextMenu_Folder(current_solution_path);

        ImGui::End();
	}
    else {
        ImGui::End();
    }

}




void Widget_FilesViewer::_OpenInExplorer(const std::string& path) {
    std::string command = "explorer /select,\"" + path + "\"";
    system(command.c_str());
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
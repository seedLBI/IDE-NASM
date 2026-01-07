#ifndef WIDGET_FILES_VIEWER_H
#define WIDGET_FILES_VIEWER_H

#include <nlohmann/json.hpp>
#include <string>

#include "ThirdParty/IconFontCppHeaders/IconsFontAwesome6.h"
#include "Utils/Text/Encoding/Wstring/Utils.Encoding.Wstring.h"

#include "IDE/Core/Widget/IWidget.h"
#include "IDE/Core/Theme/interface/IThemeLoadable.h"
#include "IDE/Core/Solution/SolutionInfo.h"

#include "IDE/GUI/Widgets/TextEditor/WidgetManager_TextEditior.h"


class Widget_FilesViewer : public IWidget, public IThemeLoadable {
private:

	char create_buffer[256] = { 0 };
	char rename_buffer[256] = { 0 };
	bool show_create_file_popup = false;
	bool show_create_folder_popup = false;
	bool need_focus_popup = false;


	std::string path_to_rename;
	bool need_focus_rename = false;

	std::string selected_context_full_path;

	struct ClipboardData {
		std::string path;
		bool is_cut = false;
		bool is_valid = false;
	};
	ClipboardData clipboard;


public:
	Widget_FilesViewer(WidgetManager_TextEditor* widgetManager_TextEditor);
	~Widget_FilesViewer();

	void LoadSolutionInfo(SolutionInfo* solutionData_ptr);

	void Draw() override;
	void Update() override;
	nlohmann::json Save() override;
	void Load(const nlohmann::json& Data) override;
	
	void LoadColors() override;
	std::vector<NamedColor> GetDefaultLightColors() override;
	std::vector<NamedColor> GetDefaultDarkColors() override;

private:
	SolutionInfo* currentSolution = nullptr;
	nlohmann::json directory_info;

	WidgetManager_TextEditor* widgetManager_TextEditor = nullptr;

	std::vector<std::string> folders_open;

	ImColor color_folder_open;
	ImColor color_folder_close;
	ImColor color_background_Project;
	ImColor color_text_Project;
	

	void _OpenPopupCreationFolder(const std::string& full_path);
	void _OpenPopupCreationFile(const std::string& full_path);
	void _SetRenameMode(const std::string& full_path);



	void _OpenInExplorer(const std::string& path);
	bool _CreateFile(const std::string& directory, const std::string& nameFile);
	bool _CreateFolder(const std::string& directory, const std::string& nameFolder);
	bool _DeletePath(const std::string& full_path);
	bool _RenamePath(const std::string& old_full_path, const std::string& newName);
	void _CopyPath(const std::string& full_path);
	void _CutPath(const std::string& full_path);
	bool _PastePath(const std::string& destination_folder);
	bool _SetAsEntryPoint(const std::string& full_path);


	enum class StateOperation {
		Nothing,
		CreationFile,
		CreationFolder,
		DeletePath,
		RenamePath,
		PastePath
	};

	StateOperation current_state = StateOperation::Nothing;

	void ProcessState();

	std::string forstate_directory;
	std::string forstate_name;
	std::string forstate_full_path;


	void SetState_DeletePath(const std::string& full_path);
	void SetState_RenamePath(const std::string& full_path);
	void SetState_CreationFile(const std::string& directory, const std::string& nameFile);
	void SetState_CreationFolder(const std::string& directory, const std::string& nameFolder);
	void SetState_PastePath(const std::string& destination_folder);

	void DrawContextMenu_File(const std::string& fullpath);
	void DrawContextMenu_Folder(const std::string& fullpath);
	void DrawPopup_CreationFolder();
	void DrawPopup_CreationFile();


	void UpdateWords();
	void UpdateDirectory();
	nlohmann::json build_directory_json(const std::wstring& path);
	void render_imgui_tree(const nlohmann::json& node);

	std::string s_open;
	std::string s_cut;
	std::string s_copy;
	std::string s_paste;
	std::string s_rename;
	std::string s_delete;
	std::string s_createFile;
	std::string s_createFolder;
	std::string s_add;
	std::string s_openInExplorer;
	std::string s_setAsEntryPoint;
};



#endif
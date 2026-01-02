#include "Widget_TextEditor.h"


TextEditor* Widget_TextEditor::GetPtrTextEditor()  {
	return textEditor;
}


void Widget_TextEditor::Draw() {
	UpdateFocus(static_cast<float>(fps_limiter->GetDeltaTime()));

	if (GetFlagShow() == false)
		return;
	


	if (textEditor->IsTextChanged())
		flags_CodeEditor = ImGuiWindowFlags_UnsavedDocument;

	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	if (targetDockId != 0) {
		ImGui::SetNextWindowDockID(targetDockId);
		targetDockId = 0;
	}

	if (ImGui::Begin(NameWidget_cstr, GetPtrFlagShow(), flags_CodeEditor)) {
		textEditor->Render(NameTextEditor_cstr);
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

void Widget_TextEditor::Update() {
	UpdateActive();
}

void Widget_TextEditor::MakeActiveCurrentWidget() {
	ImGui::SetWindowFocus(GetName().c_str());
	ImGui::SetWindowFocus(GetPtrTextEditor()->GetNameChild().c_str());
}


nlohmann::json Widget_TextEditor::Save() {
	nlohmann::json SaveData;
		SaveData["flag_Active"] = GetFlagShow();
	return SaveData;
}

void Widget_TextEditor::Load(const nlohmann::json& SaveData) {

	if (SaveData.contains("flag_Active")){
		SetFlagShow(SaveData["flag_Active"].get<bool>());
	}

}

std::string Widget_TextEditor::GetNameWidget()
{
	return NameWidget;
}

std::wstring Widget_TextEditor::GetFilePath() {
	return File_Path;
}

void Widget_TextEditor::SaveFile() {
	flags_CodeEditor = 0;
	textEditor->SaveFile();
}

void Widget_TextEditor::SetTargetDockID(const ImGuiID& id){
	this->targetDockId = id;
}


void Widget_TextEditor::ChangeFullPath(const std::wstring& newFullPath) {
	File_Path = newFullPath;
	File_Name = newFullPath.substr(newFullPath.find_last_of(L"\\"));

	NameWidget = wstring_to_stringUTF8(File_Name) + "##" + std::to_string(index);
	NameWidget_cstr = NameWidget.c_str();
	SetName(NameWidget);

	NameTextEditor = NameWidget + u8"TextEditor";
	NameTextEditor_cstr = NameTextEditor.c_str();


}


Widget_TextEditor::Widget_TextEditor(FPS_Timer* fps_limiter, const std::wstring& FullPath, const int& index) : IWidget("") {
	this->fps_limiter = fps_limiter;
	textEditor = new TextEditor(fps_limiter, FullPath);


	textEditor->SetTextNotChanged();

	File_Path = FullPath;
	File_Name = FullPath.substr(FullPath.find_last_of(L"\\") + 1);

	this->index = index;

	NameWidget = wstring_to_stringUTF8(File_Name) + "##" + std::to_string(index);
	NameWidget_cstr = NameWidget.c_str();
	SetName(NameWidget);

	NameTextEditor = NameWidget + u8"TextEditor";
	NameTextEditor_cstr = NameTextEditor.c_str();
}



Widget_TextEditor::~Widget_TextEditor() {
#ifdef _DEBUG
	std::cout << "ENTER -> Widget_TextEditor::~Widget_TextEditor()\n";
#endif

	textEditor->~TextEditor();

#ifdef _DEBUG
	std::cout << "LEAVE -> Widget_TextEditor::~Widget_TextEditor()\n";
#endif
}
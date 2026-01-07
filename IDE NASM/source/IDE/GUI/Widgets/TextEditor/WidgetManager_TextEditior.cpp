#include "WidgetManager_TextEditior.h"


WidgetManager_TextEditor::WidgetManager_TextEditor(FPS_Timer* fps_limiter, PositionWidgetsManager* positionsWidgetsManager):
	IWidget("widgetName.managerTextEditor"),
	IThemeLoadable("themeItem.editorCode")
{
	this->fps_limiter = fps_limiter;
	this->positionsWidgetsManager = positionsWidgetsManager;

	IThemeLoadable::InitListWord(
		{
			"color.textEditor.default",
			"color.textEditor.keyword",
			"color.textEditor.number",
			"color.textEditor.string",
			"color.textEditor.charLiteral",
			"color.textEditor.punctuation",
			"color.textEditor.identifier",
			"color.textEditor.comment",
			"color.textEditor.multiLineComment",
			"color.textEditor.background",
			"color.textEditor.cursor",
			"color.textEditor.selection",
			"color.textEditor.errorMarker",
			"color.textEditor.breakpoint",
			"color.textEditor.lineNumber",
			"color.textEditor.currentLineFill",
			"color.textEditor.currentLineFillInactive",
			"color.textEditor.currentLineEdge",
			"color.textEditor.finderNotFind"
		});
}

WidgetManager_TextEditor::~WidgetManager_TextEditor() {

}

void WidgetManager_TextEditor::LoadColors() {
	for (int i = 0; i < widgets.size(); i++) {
		widgets[i]->GetPtrTextEditor()->LoadColors(object_colors);
	}
}
std::vector<NamedColor> WidgetManager_TextEditor::GetDefaultLightColors() {
	std::vector<NamedColor> colors = {
		{"color.textEditor.default",				ImColor(125,115,104,255)},
		{"color.textEditor.keyword",				ImColor(6,12,255,255)},
		{"color.textEditor.number",					ImColor(80,109,1,255)},
		{"color.textEditor.string",					ImColor(160,32,32,255)},
		{"color.textEditor.charLiteral",			ImColor(112,64,48,255)},
		{"color.textEditor.punctuation",			ImColor(71,41,0,255)},
		{"color.textEditor.identifier",				ImColor(71,41,0,255)},
		{"color.textEditor.comment",				ImColor(8,84,8,255)},
		{"color.textEditor.multiLineComment",		ImColor(11,84,60,255)},
		{"color.textEditor.background",				ImColor(232,222,203,255)},
		{"color.textEditor.cursor",					ImColor(0,0,0,255)},
		{"color.textEditor.selection",				ImColor(242,158,0,79)},
		{"color.textEditor.errorMarker",			ImColor(255,16,0,160)},
		{"color.textEditor.breakpoint",				ImColor(240,163,0,128)},
		{"color.textEditor.lineNumber",				ImColor(112,0,0,255)},
		{"color.textEditor.currentLineFill",		ImColor(37,19,0,64)},
		{"color.textEditor.currentLineFillInactive",ImColor(128,117,97,64)},
		{"color.textEditor.currentLineEdge",		ImColor(64,38,0,64)},
		{"color.textEditor.finderNotFind",			ImColor(201,178,133,255) }
	};


	return colors;
}
std::vector<NamedColor> WidgetManager_TextEditor::GetDefaultDarkColors() {
	std::vector<NamedColor> colors = {
		{"color.textEditor.default",				ImColor(0xff7f7f7f)},
		{"color.textEditor.keyword",				ImColor(0xffff0c06)},
		{"color.textEditor.number",					ImVec4(0.2f,1.0f,0.2f,1.0f)},
		{"color.textEditor.string",					ImColor(0xff7070e0)},
		{"color.textEditor.charLiteral",			ImColor(0xff70a0e0)},
		{"color.textEditor.punctuation",			ImColor(0xffffffff)},
		{"color.textEditor.identifier",				ImColor(0xffaaaaaa)},
		{"color.textEditor.comment",				ImColor(0xff206020)},
		{"color.textEditor.multiLineComment",		ImColor(0xff406020)},
		{"color.textEditor.background",				ImVec4(0.15f, 0.15f, 0.15f,1.0f)},
		{"color.textEditor.cursor",					ImColor(0xffe0e0e0)},
		{"color.textEditor.selection",				ImColor(0x80a06020)},
		{"color.textEditor.errorMarker",			ImColor(0x800020ff)},
		{"color.textEditor.breakpoint",				ImVec4(0.8f, 0.6f, 0.2f,1.0f)},
		{"color.textEditor.lineNumber",				ImColor(0xff707000)},
		{"color.textEditor.currentLineFill",		ImColor(0x40000000)},
		{"color.textEditor.currentLineFillInactive",ImColor(0x40808080)},
		{"color.textEditor.currentLineEdge",		ImColor(0x40a0a0a0)},
		{"color.textEditor.finderNotFind",			ImColor(0.2f, 0.2f, 0.2f)}
	};


	return colors;
}


Widget_TextEditor* WidgetManager_TextEditor::GetFocusedTextEditor() {
	for (int i = 0; i < widgets.size(); i++) {

		if (widgets[i]->GetPtrTextEditor()->GetIsFocused())
			return widgets[i];

	}
	return nullptr;
}

Widget_TextEditor* WidgetManager_TextEditor::GetLastFocusedTextEditor(){

	for (int i = 0; i < widgets.size(); i++) {

		if (widgets[i]->GetName() == LastActiveWidget)
			return widgets[i];

	}

	if (widgets.empty() == false)
		return widgets.front();

	return nullptr;
}


std::vector<std::string> WidgetManager_TextEditor::GetNamesTextEditorChild() {
	std::vector<std::string> result;
	
	for (int i = 0; i < widgets.size(); i++) {

		if (widgets[i]->GetPtrTextEditor()->GetNameChild().empty())
			continue;

		result.push_back(widgets[i]->GetPtrTextEditor()->GetNameChild());
	}


	return result;
}
std::vector<Widget_TextEditor*> WidgetManager_TextEditor::GetWidgets() {
	return widgets;
}


void WidgetManager_TextEditor::SaveCurrentActiveFile() {

	if (GetFocusedTextEditor() == nullptr)
		return;

	GetFocusedTextEditor()->SaveFile();

}

void WidgetManager_TextEditor::SaveAllFile() {

	for (int i = 0; i < widgets.size(); i++) {
		widgets[i]->SaveFile();
	}

}

void WidgetManager_TextEditor::ClearAll() {

	for (int i = 0; i < widgets.size(); i++){
		widgets[i]->GetPtrTextEditor()->SaveFile();
	}
	LastActiveWidget.clear();
	widgets.clear();
}


void WidgetManager_TextEditor::CloseAll() {
	for (int i = 0; i < widgets.size(); i++) {
		widgets[i]->SetFlagShow(false);
	}
}


void WidgetManager_TextEditor::SetActiveFromPath(const std::wstring& Path) {


	for (int i = 0; i < widgets.size(); i++) {
		if (widgets[i]->GetFilePath() == Path) {

			if (widgets[i]->GetFlagShow() == false) {

				auto last_focused_text_editor = GetLastFocusedTextEditor();

				widgets[i]->SetFlagShow(true);

				if (last_focused_text_editor != nullptr && last_focused_text_editor != widgets[i]){
					ImGuiWindow* existingEditor = ImGui::FindWindowByName(last_focused_text_editor->GetNameWidget().c_str());
					ImGuiID targetDockId = 0;
					if (existingEditor != nullptr) {
						targetDockId = existingEditor->DockId;
					}


					widgets[i]->SetTargetDockID(targetDockId);
				}
				else {
					widgets[i]->SetTargetDockID(positionsWidgetsManager->GetMainDockID());
				}

			}

			
			widgets[i]->MakeActiveCurrentWidget();
			break;
		}
	}

}

void WidgetManager_TextEditor::Update() {
	UpdateFocus(static_cast<float>(fps_limiter->GetDeltaTime()));
	UpdateActive();

	for (size_t i = 0; i < widgets.size(); i++) {
		if (widgets[i]->GetPtrTextEditor()->GetIsFocused()) {
			LastActiveWidget = widgets[i]->GetName();
		}
	}


}

void WidgetManager_TextEditor::Draw() {

	for (int i = 0; i < widgets.size(); i++){
		widgets[i]->Draw();
	}

}

void WidgetManager_TextEditor::LoadSolutionInfo(SolutionInfo* solutionData_ptr) {
	this->currentSolution = solutionData_ptr;
}

void WidgetManager_TextEditor::Add(const std::wstring& Path) {

	widgets.push_back(new Widget_TextEditor(fps_limiter, Path, static_cast<int>(widgets.size())));
	LoadColors();
}

void WidgetManager_TextEditor::Remove(const std::wstring& Path) {
	for (size_t i = 0; i < widgets.size(); i++) {
		if (widgets[i]->GetFilePath() == Path) {
			widgets.erase(widgets.begin() + i);
			break;
		}
	}
}




Widget_TextEditor* WidgetManager_TextEditor::GetWidgetFromPath(const std::wstring& Path) {
	for (int i = 0; i < widgets.size(); i++) {
		if (widgets[i]->GetFilePath() == Path) {
			return widgets[i];
		}
	}
	return nullptr;
}

nlohmann::json WidgetManager_TextEditor::Save() {

	nlohmann::json result;

	std::string path_solution = wstring_to_stringUTF8(currentSolution->GetPathAbsolute() + L"\\" + currentSolution->GetPathSource() + L"\\");


	for (size_t i = 0; i < widgets.size(); i++) {

		std::string path_absolute = wstring_to_stringUTF8(widgets[i]->GetFilePath());

		std::string path_relative = path_absolute;


		RemoveSubstringFromStart(path_relative, path_solution);


		result["TextEditors"][path_relative] = widgets[i]->Save();
	}

	return result;
}
void WidgetManager_TextEditor::Load(const nlohmann::json& Data) {

	std::string path_solution = wstring_to_stringUTF8(currentSolution->GetPathAbsolute() + L"\\" + currentSolution->GetPathSource() + L"\\");


	if (Data.contains("TextEditors")) {
		for (auto& [key, value] : Data["TextEditors"].items()) {
			auto widget = GetWidgetFromPath(stringUTF8_to_wstring(path_solution + key));
			if(widget)
				widget->Load(value);
		}

	}
}
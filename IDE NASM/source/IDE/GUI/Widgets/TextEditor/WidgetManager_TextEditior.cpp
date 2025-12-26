#include "WidgetManager_TextEditior.h"


WidgetManager_TextEditor::WidgetManager_TextEditor(FPS_Timer* fps_limiter): IWidget(u8"Менеджер редакторов кода"), IThemeLoadable(u8"Редактор кода") {
	this->fps_limiter = fps_limiter;

	IThemeLoadable::InitListWord(
		{
			u8"Ничего",u8"Ключевые",u8"Числа",u8"Строки (\"\")",u8"Символы (\'\')",
			u8"Пунктуация",u8"Индификаторы",u8"Комментарии (однострочные)",
			u8"Комментарии (многострочные)",u8"Фон",u8"Курсор",u8"Выделение",
			u8"Ошибка",u8"Точка останова",u8"Номер строки",u8"Строка (активная)",
			u8"Строка (не активная)",u8"Строка (границы)",u8"Поиск кнопки (слово не нашёл)"
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
		{u8"Ничего",						ImColor(125,115,104,255)},
		{u8"Ключевые",						ImColor(6,12,255,255)},
		{u8"Числа",							ImColor(80,109,1,255)},
		{u8"Строки (\"\")",					ImColor(160,32,32,255)},
		{u8"Символы (\'\')",				ImColor(112,64,48,255)},
		{u8"Пунктуация",					ImColor(71,41,0,255)},
		{u8"Индификаторы",					ImColor(71,41,0,255)},
		{u8"Комментарии (однострочные)",	ImColor(8,84,8,255)},
		{u8"Комментарии (многострочные)",	ImColor(11,84,60,255)},
		{u8"Фон",							ImColor(232,222,203,255)},
		{u8"Курсор",						ImColor(0,0,0,255)},
		{u8"Выделение",						ImColor(242,158,0,79)},
		{u8"Ошибка",						ImColor(255,16,0,160)},
		{u8"Точка останова",				ImColor(240,163,0,128)},
		{u8"Номер строки",					ImColor(112,0,0,255)},
		{u8"Строка (активная)",				ImColor(37,19,0,64)},
		{u8"Строка (не активная)",			ImColor(128,117,97,64)},
		{u8"Строка (границы)",				ImColor(64,38,0,64)},
		{u8"Поиск кнопки (слово не нашёл)",	ImColor(201,178,133,255) }
	};


	return colors;
}
std::vector<NamedColor> WidgetManager_TextEditor::GetDefaultDarkColors() {
	std::vector<NamedColor> colors = {
		{u8"Ничего",							ImColor(0xff7f7f7f)},
		{u8"Ключевые",							ImColor(0xffff0c06)},
		{u8"Числа",								ImVec4(0.2f,1.0f,0.2f,1.0f)},
		{u8"Строки (\"\")",						ImColor(0xff7070e0)},
		{u8"Символы (\'\')",					ImColor(0xff70a0e0)},
		{u8"Пунктуация",						ImColor(0xffffffff)},
		{u8"Индификаторы",						ImColor(0xffaaaaaa)},
		{u8"Комментарии (однострочные)",		ImColor(0xff206020)},
		{u8"Комментарии (многострочные)",		ImColor(0xff406020)},
		{u8"Фон",								ImVec4(0.15f, 0.15f, 0.15f,1.0f)},
		{u8"Курсор",							ImColor(0xffe0e0e0)},
		{u8"Выделение",							ImColor(0x80a06020)},
		{u8"Ошибка",							ImColor(0x800020ff)},
		{u8"Точка останова",					ImVec4(0.8f, 0.6f, 0.2f,1.0f)},
		{u8"Номер строки",						ImColor(0xff707000)},
		{u8"Строка (активная)",					ImColor(0x40000000)},
		{u8"Строка (не активная)",				ImColor(0x40808080)},
		{u8"Строка (границы)",					ImColor(0x40a0a0a0)},
		{u8"Поиск кнопки (слово не нашёл)",		ImColor(0.2f, 0.2f, 0.2f)}
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


void WidgetManager_TextEditor::SetActiveFromPath(const std::wstring& Path) {


	for (int i = 0; i < widgets.size(); i++) {
		if (widgets[i]->GetFilePath() == Path) {
			widgets[i]->SetFlagShow(true);
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

void WidgetManager_TextEditor::Add(const std::wstring& Path) {

	widgets.push_back(new Widget_TextEditor(fps_limiter, Path, static_cast<int>(widgets.size())));
	LoadColors();
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

	for (size_t i = 0; i < widgets.size(); i++) {
		result["TextEditors"][wstring_to_stringUTF8(widgets[i]->GetFilePath())] = widgets[i]->Save();
	}

	return result;
}
void WidgetManager_TextEditor::Load(const nlohmann::json& Data) {

	if (Data.contains("TextEditors")) {
		for (auto& [key, value] : Data["TextEditors"].items()) {
			GetWidgetFromPath(stringUTF8_to_wstring(key))->Load(value);
		}

	}
}
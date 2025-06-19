#ifndef TEXT_EDITOR_WIDGET_H
#define TEXT_EDITOR_WIDGET_H


#include "IDE/Core/Widget/IWidget.h"

#include "Utils/Text/Encoding/Wstring/Utils.Encoding.Wstring.h"

#include "IDE/GUI/Widgets/TextEditor/TextEditor.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>


class Widget_TextEditor : public IWidget
{
public:
	Widget_TextEditor(FPS_Timer* fps_limiter, const std::wstring& FullPath, const int& index);
	~Widget_TextEditor();

	void Draw() override;
	void Update() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& SaveData) override;

	void ChangeFullPath(const std::wstring& newFullPath);

	void MakeActiveCurrentWidget() override;


	void SaveFile();


	std::wstring GetFilePath();

	TextEditor* GetPtrTextEditor();

private:
	FPS_Timer* fps_limiter = nullptr;
	int index = 0;

	ImGuiWindowFlags flags_CodeEditor = 0;

	bool flag_Active = false;

	std::string NameWidget;
	const char* NameWidget_cstr;

	std::string NameTextEditor;
	const char* NameTextEditor_cstr;

	TextEditor* textEditor = nullptr;
	std::wstring File_Path;
	std::wstring File_Name;
};



#endif //!TEXT_EDITOR_WIDGET_H
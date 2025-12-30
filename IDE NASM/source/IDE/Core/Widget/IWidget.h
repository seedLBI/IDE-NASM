#ifndef IWIDGET_H
#define IWIDGET_H

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"
#include "IDE/Core/SaveSystem/SaveSystem.h"
#include "Utils/Text/Encoding/UTF8/Utils.Encoding.UTF8.h"
#include <string>

class IWidget : public SaveSystem
{
public:
	IWidget(const std::string& Name, const bool& flag = false);
	~IWidget();

	void virtual Draw();
	void virtual Update();
	nlohmann::json Save() override;
	void Load(const nlohmann::json& Data) override;


	void UpdateActive();

	bool GetFlagShow();
	void SetFlagShow(const bool& value);
	bool* GetPtrFlagShow();

	void SetFocus();
	void UpdateFocus(const float& deltaTime);

	void virtual MakeActiveCurrentWidget();
	void SetActive();

	bool WindowIsVisiable();

	float GetTimerFocus();

	std::string GetNameTranslated();
	std::string GetName();

	void SetName(const std::string& newName);

	virtual std::string GetCommand();




protected:
	float TimerFocus = 0.f;
	bool flag_FocusSeted = false;

private:
	std::string Name;
	bool* Show;
	int countSet = 0;
	bool IsSettedActive;

};

#endif // !WIDGET_H

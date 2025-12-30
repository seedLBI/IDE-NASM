#include "WidgetManager.h"

WidgetManager::WidgetManager(FPS_Timer* fps_limiter) : SaveSystem("Widgets")
{
	this->fps_limiter = fps_limiter;
}


WidgetManager::~WidgetManager()
{
#ifdef WITH_DEBUG_OUTPUT
	std::cout << "I8080_WidgetManager::~I8080_WidgetManager()\n";
#endif // !WITH_DEBUG_OUTPUT
}


void WidgetManager::Draw() {
	for (int i = 0; i < ptr_widgets.size(); i++) {


		ptr_widgets[i]->UpdateFocus(float(fps_limiter->GetDeltaTime()));
		ptr_widgets[i]->Draw();


		float TimerFocus = ptr_widgets[i]->GetTimerFocus()* ptr_widgets[i]->GetTimerFocus();
		if (TimerFocus > 0.f) {

			ImGuiWindow* window = ImGui::FindWindowByName(ptr_widgets[i]->GetName().c_str());

			auto drawlist = window->DrawList;

			drawlist->AddRectFilled(
				window->Pos,
				{ window->Pos.x + window->Size.x,window->Pos.y + window->Size.y },
				ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, TimerFocus, 0.f, TimerFocus)));
		}
	}
}
void WidgetManager::Update() {

	for (int i = 0; i < ptr_widgets.size(); i++)
		ptr_widgets[i]->Update();

	if (indexHelp == -1)
		return;

	std::string NamePressedButtonFromHelp = ptr_widgets[indexHelp]->GetCommand();
	if (NamePressedButtonFromHelp.empty())
		return;

	int index = GetIndexSaveObject(NamePressedButtonFromHelp);

	if (ptr_widgets[index]->GetFlagShow() == false)
		ptr_widgets[index]->SetFlagShow(true);
	else
		ptr_widgets[index]->SetFocus();

}

int WidgetManager::FindIndex(std::string nameWidget) {
	for (int i = 0; i < ptr_widgets.size(); i++) {
		if (ptr_widgets[i]->GetName() == nameWidget)
			return i;
	}
	return -1;
}

void WidgetManager::AddWidgetPtr(IWidget* ptr_widget) {
	ptr_widgets.emplace_back(ptr_widget);
	if (ptr_widget->GetName() == u8"Руководство")
		indexHelp = int(ptr_widgets.size()) - 1;
}

IWidget* WidgetManager::GetWidgetPtrByName(const std::string& Name) {
	int index = FindIndex(Name);
	if (index != -1)
		return ptr_widgets[index];
	else
		return nullptr;
}


void WidgetManager::LoadVisibilitySettings(std::vector<std::pair<std::string, bool>> settingData) {
	for (int i = 0; i < settingData.size(); i++) {
		int index = FindIndex(settingData[i].first);
		if (index != -1)
			ptr_widgets[index]->SetFlagShow(settingData[i].second);
	}
}

std::vector<std::pair<std::string, bool>> WidgetManager::GetVisibilitySettings() {
	std::vector<std::pair<std::string, bool>> result;
	for (int i = 0; i < ptr_widgets.size(); i++){
		result.emplace_back(std::pair<std::string, bool>{ ptr_widgets[i]->GetName(),ptr_widgets[i]->GetFlagShow() });
	}
	return result;
}

int WidgetManager::GetIndexSaveObject(const std::string& NameObject) {
	for (int i = 0; i < ptr_widgets.size(); i++){
		if (ptr_widgets[i]->GetName() == NameObject)
			return i;
	}
	std::cout << "Don't have widget object with Name: " << NameObject << std::endl;
	//exit(545);
	return -1;
}


nlohmann::json WidgetManager::Save() {
	nlohmann::json SaveData;

	for (int i = 0; i < ptr_widgets.size(); i++)
		SaveData[ptr_widgets[i]->GetName()] = ptr_widgets[i]->Save();

	return SaveData;
}

void WidgetManager::Load(const nlohmann::json& Data) {

	for (auto& [widgetName, widgetSaveData] : Data.items()) {
		int index_object_widget = GetIndexSaveObject(widgetName);
		if (index_object_widget == -1)
			continue;
		ptr_widgets[index_object_widget]->Load(widgetSaveData);
	}
}


bool WidgetManager::GetFlag_AllOpened() {

	for (int i = 0; i < ptr_widgets.size(); i++)
	{
		if (ptr_widgets[i]->GetFlagShow() == false)
		{
			return false;
		}
	}

	return true;
}

void WidgetManager::CloseAll() {
	for (int i = 0; i < ptr_widgets.size(); i++)
		ptr_widgets[i]->SetFlagShow(false);
}
void WidgetManager::OpenAll() {
	for (int i = 0; i < ptr_widgets.size(); i++)
		ptr_widgets[i]->SetFlagShow(true);
}
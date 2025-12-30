#include "IWidget.h"
#include "IDE/Core/LocalisationManager/LocalisationManager.h"

IWidget::IWidget(const std::string& Name, const bool& flag) : SaveSystem(Name){
#ifdef _DEBUG
	std::cout << "IWidget::IWidget(const std::string& Name, const bool& flag) : SaveSystem(Name)\n";
#endif
	this->Name = Name;
	Show = new bool;
	*Show = flag;
}

IWidget::~IWidget() {
#ifdef _DEBUG
	std::cout << "IWidget::~IWidget()\n";
#endif
}

void IWidget::Draw() {

}

void IWidget::Update() {

}
void IWidget::UpdateActive() {
	if (IsSettedActive){
		IsSettedActive = false;
		MakeActiveCurrentWidget();
	}
}

void IWidget::SetFocus() {
	TimerFocus = 1.f;
	flag_FocusSeted = true;
}

void IWidget::SetActive() {

	IsSettedActive = true;
}

void IWidget::MakeActiveCurrentWidget() {
	ImGui::SetWindowFocus(Name.c_str());
}

bool IWidget::WindowIsVisiable() {

	ImGuiContext& g = *GImGui;

	for (ImGuiWindow* window : g.Windows) {
		if (std::string(window->Name) == Name && window->Hidden == false) {
			return true;
		}
	}

	return false;
}

// Это добавлять в Draw
void IWidget::UpdateFocus(const float& deltaTime) {
	if (flag_FocusSeted){
		MakeActiveCurrentWidget();
		flag_FocusSeted = false;
	}
	TimerFocus -= deltaTime;
	if (TimerFocus < 0.f)
		TimerFocus = 0.f;
}

float IWidget::GetTimerFocus() {
	return TimerFocus;
}

std::string IWidget::GetCommand() {
	return "";
}

bool IWidget::GetFlagShow() {
	return *Show;
}

bool* IWidget::GetPtrFlagShow() {
	return Show;
}

void IWidget::SetFlagShow(const bool& value) {
	*Show = value;
}


std::string IWidget::GetName() {
	return Name;
}

std::string IWidget::GetNameTranslated() {
	return tr(Name) + "###" + Name;
}

void IWidget::SetName(const std::string& newName) {
	Name = newName;
}


nlohmann::json IWidget::Save() {
#ifdef _DEBUG
	std::cout << "nlohmann::json IWidget::Save()\n";
#endif // _DEBUG
	return nlohmann::json();
}

void IWidget::Load(const nlohmann::json& Data) {
#ifdef _DEBUG
	std::cout << "void IWidget::Load(const nlohmann::json& Data) \n";
#endif // _DEBUG
}
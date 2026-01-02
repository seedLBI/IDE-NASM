#include "ISettingObject.h"

#include "IDE/Core/LocalisationManager/LocalisationManager.h"

ISettingObject::ISettingObject(const std::string& NameObject, const std::string& NameTab) : ISettingSavable(NameObject), ISettingDrawable(NameTab) {

}

void ISettingObject::DrawBegin() {
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		ImGui::SeparatorText(tr(GetSaveObjectName()).c_str());
	ImGui::PopFont();
}
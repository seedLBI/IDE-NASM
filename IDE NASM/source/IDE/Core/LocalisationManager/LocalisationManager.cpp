#include "LocalisationManager.h"


#include <fstream>
#include <iostream>
#include <map>

LocalisationManager& LocalisationManager::getInstance() {
	static LocalisationManager locals;
	return locals;
}

void LocalisationManager::setLanguage(const std::string& language_abbriv){
	GlobalLanguage = language_abbriv;
}

const std::string& LocalisationManager::getGlobal() {
	return GlobalLanguage;
}

std::string LocalisationManager::translate(const std::string& keyName) {
	if (!translate_data.contains(keyName))
		return keyName;

	if (!translate_data[keyName].contains(GlobalLanguage))
		return translate_data[keyName]["en"];

	return translate_data[keyName][GlobalLanguage].get<std::string>();
}

void LocalisationManager::loadFromFile(const std::string& path) {
	std::ifstream ifn(path);
	translate_data = nlohmann::json::parse(ifn);
	ifn.close();
	UpdateSetLanguages();

	ifn.open("resources\\json\\LanguageChoose.json");
	translator_abriv = nlohmann::json::parse(ifn);
	ifn.close();

	

	
#ifdef _DEBUG
	for (const auto& [keyName, keyTranslations]: translate_data.items())
	{
		std::cout << keyName << std::endl;

		for (const auto& [abbriv, translation]: keyTranslations.items())
		{
			printf("\t\"%s\": %s\n", abbriv.c_str(), translation.get<std::string>().c_str());
		}

	}
#endif

}


void LocalisationManager::UpdateSetLanguages() {

	std::map<std::string, int> counts;
	countWords = 0;
	for (const auto& [key, langsTranslate] : translate_data.items()) {
		countWords++;

		for (const auto& [lang, translateString] : langsTranslate.items()) {
			counts[lang]++;
		}
	}

	for (const auto& pair : counts) {
		const std::string& lang = pair.first;
		const int& count_word = pair.second;

		float percent_translated = float(count_word) / (float)countWords;


		LanguageInfo info;
		info.abriv = lang;
		info.percent_translated_interface = percent_translated;

		langs_info.emplace_back(info);
	}



}



nlohmann::json LocalisationManager::SaveSetting() {
	nlohmann::json data;

	data["language"] = GlobalLanguage;

	return data;
}

void LocalisationManager::LoadSetting(const nlohmann::json& Data) {
	if (Data.contains("language"))
		GlobalLanguage = Data["language"].get<std::string>();

	UpdateIndexCurrentLanguage();
}

void LocalisationManager::UpdateIndexCurrentLanguage() {
	index_current_language = 0;
	for (size_t i = 0; i < langs_info.size(); i++)
	{
		if (GlobalLanguage == langs_info[i].abriv)
		{
			index_current_language = i;
			break;
		}
	}
}

void LocalisationManager::SetNextLanguage() {
	
	int index = (index_current_language + 1) % langs_info.size();


	GlobalLanguage = langs_info[index].abriv;
	index_current_language = index;
	
}
void LocalisationManager::SetPrevLanguage() {
	int index = index_current_language == 0? langs_info.size() - 1 : (index_current_language - 1);

	GlobalLanguage = langs_info[index].abriv;
	index_current_language = index;
}

void LocalisationManager::DrawSetting() {
	ISettingObject::DrawBegin();



	auto GetTranslationColor = [](float percent) -> ImVec4
		{
			percent = ImClamp(percent, 0.0f, 100.0f);
			float t = percent / 100.0f;

			return ImLerp(ImVec4(0.5f, 0.1f, 0.1f, 1.0f),
						  ImVec4(0.1f, 0.5f, 0.1f, 1.0f),   // ярко-зелёный при 100%
				t);
		};


	for (size_t i = 0; i < langs_info.size(); i++){

	
		std::string language_name = langs_info[i].abriv;

		if (translator_abriv.contains(langs_info[i].abriv)) {
			language_name = translator_abriv[langs_info[i].abriv]["description"].get<std::string>();
		}
		


		bool is_selected = (langs_info[i].abriv == GlobalLanguage);
		if (ImGui::RadioButton(language_name.c_str(), is_selected)){
			GlobalLanguage = langs_info[i].abriv;
		}

		float percent = langs_info[i].percent_translated_interface * 100.0f;
		ImVec4 color = GetTranslationColor(percent);


		ImGui::SameLine();

		if (percent >= 99.0f) {
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
				ImGui::TextColored(color, "%.0f%%", percent);
			ImGui::PopFont();
		}
		else {
			ImGui::TextColored(color, "%.0f%%", percent);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_SeparatorTextBorderSize, 4.f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 5));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

				ImGui::BeginTooltip();
				ImGui::Text(u8"%s: %s", tr("localisationManager.setting.tooltip.code").c_str(), langs_info[i].abriv.c_str());
				ImGui::Text(u8"%s: %.1f%%", tr("localisationManager.setting.tooltip.translateInterface").c_str(), percent);
				if (percent < 100.0f)
					ImGui::TextColored(ImVec4(0.5f, 0.1f, 0.1f, 1.0f), tr("localisationManager.setting.tooltip.translate.fail").c_str());
				else {
					ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
					ImGui::TextColored(ImVec4(0.1f, 0.5f, 0.1f, 1.0f), tr("localisationManager.setting.tooltip.translate.success").c_str());
					ImGui::PopFont();
				}
				ImGui::EndTooltip();

			ImGui::PopStyleVar(3);
		}
		
		
	}


}



LocalisationManager::LocalisationManager(): ISettingObject("setting.name.Language", "setting.tabName.general") {
	loadFromFile("resources\\json\\Locales.json");
}




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
}

void LocalisationManager::DrawSetting() {
	ISettingObject::DrawBegin();

	for (size_t i = 0; i < langs_info.size(); i++)
	{
		ImGui::Text("%3.1f%%", langs_info[i].percent_translated_interface * 100.f);
		ImGui::SameLine();
		if (ImGui::Selectable(langs_info[i].abriv.c_str(), langs_info[i].abriv == GlobalLanguage)) {
			GlobalLanguage = langs_info[i].abriv;
		}
		
		
	}


}



LocalisationManager::LocalisationManager(): ISettingObject("setting.name.Language", "setting.tabName.general") {
	loadFromFile("resources\\json\\Locales.json");
}




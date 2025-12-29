#include "LocalisationManager.h"


#include <fstream>
#include <iostream>

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

	if (translate_data.contains(keyName))
		return translate_data[keyName][GlobalLanguage].get<std::string>();
	else
		return keyName;
}

void LocalisationManager::loadFromFile(const std::string& path) {
	std::ifstream ifn(path);
	translate_data = nlohmann::json::parse(ifn);
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


nlohmann::json LocalisationManager::SaveSetting() {
	return {};
}

void LocalisationManager::LoadSetting(const nlohmann::json& Data) {

}

void LocalisationManager::DrawSetting() {

}



LocalisationManager::LocalisationManager(const std::string& path): ISettingObject(u8"ќбщие", u8"язык") {
	loadFromFile(path);
}




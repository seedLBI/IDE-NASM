#ifndef LOCALISATION_MANAGER_H
#define LOCALISATION_MANAGER_H

#include "IDE/Core/Setting/interfaces/ISettingObject.h"

#include <nlohmann/json.hpp>

#include <string>

class LocalisationManager : public ISettingObject { 

private:
	nlohmann::json translate_data;

	std::string GlobalLanguage = u8"en";

public:
	static LocalisationManager& getInstance();

	void setLanguage(const std::string& language_abbriv);
	std::string translate(const std::string& keyName);

	void loadFromFile(const std::string& path);


	nlohmann::json SaveSetting() override;
	void LoadSetting(const nlohmann::json& Data) override;
	void DrawSetting() override;



private:
	LocalisationManager(const std::string& path = "resources\\json\\Locales.json");
	LocalisationManager(const LocalisationManager&) = delete;
	LocalisationManager& operator=(const LocalisationManager&) = delete;
};


inline std::string tr(const std::string& keyName) {
	return LocalisationManager::getInstance().translate(keyName);
}



#endif
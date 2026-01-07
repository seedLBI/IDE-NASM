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

	const std::string& getGlobal();
	void setLanguage(const std::string& language_abbriv);
	std::string translate(const std::string& keyName);

	void loadFromFile(const std::string& path);


	nlohmann::json SaveSetting() override;
	void LoadSetting(const nlohmann::json& Data) override;
	void DrawSetting() override;

	void SetNextLanguage();
	void SetPrevLanguage();



private:
	LocalisationManager();
	LocalisationManager(const LocalisationManager&) = delete;
	LocalisationManager& operator=(const LocalisationManager&) = delete;

	nlohmann::json translator_abriv;

	struct LanguageInfo {
		std::string abriv;
		float percent_translated_interface = 0.f;
	};
	std::vector<LanguageInfo> langs_info;
	int countWords = 0;
	void UpdateSetLanguages();

	int index_current_language = -1;
	void UpdateIndexCurrentLanguage();
};


inline std::string tr(const std::string& keyName) {
	return LocalisationManager::getInstance().translate(keyName);
}

inline std::string gl() {
	return LocalisationManager::getInstance().getGlobal();
}

#endif
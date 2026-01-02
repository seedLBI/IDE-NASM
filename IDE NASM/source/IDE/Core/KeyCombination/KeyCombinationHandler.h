#include "KeyCombination.h"

#include "IDE/GUI/Elements/Notification/NotificationManager.h"

#include "IDE/Core/SaveSystem/SaveSystem.h"
#include "IDE/Core/Setting/interfaces/ISettingObject.h"

#include "Utils/Keyboard/Keyboard.Utils.h"
#include "Utils/ImGui/ImGui.Utils.h"
#include "Utils/Timer/Utils.Timer.Framerate.h"
#include "Utils/OpenGL/Window/Window.h"

#include <functional>
#include <iostream>



#ifndef KEY_COMBINATION_HANDLER_H
#define KEY_COMBINATION_HANDLER_H





class KeyCombinationHandler : public ISettingObject
{
public:

	KeyCombinationHandler(NotificationManager* notificationManager, FPS_Timer* fps_limiter, Window* MainWindow);
	~KeyCombinationHandler();
	
	
	bool GetCollision(const KeyCombination& comb);

	bool AddCombination(const std::string& Name, const KeyCombination& comb);

	void Update();

	std::string GetStrCombinationByName(const std::string& name);


	bool PopupIsOpen();


	void DrawSetting() override;

	nlohmann::json SaveSetting() override;
	void LoadSetting(const nlohmann::json& Data) override;


private:

	NotificationManager* notificationManager = nullptr;
	FPS_Timer* fps_limiter = nullptr;
	Window* MainWindow = nullptr;

	nlohmann::json default_settings;


	std::vector<std::pair<std::string, KeyCombination>> combinations;
	int GetIndexCombinationByName(const std::string& name);
	int GetIndexCollision(const KeyCombination& comb);
	int GetIndexCollision(const std::vector<int>& comb);

	std::vector<Key> KeysToCheck;
	int GetIndexKeyByNumber(const int& number);
	std::vector<Key> GetUniqueKeys();


	bool flag_StopProcess = false;
	void ProcessAllFunctions();
	void ProcessAllUniqueKeys();


	int SelectedCombination_For_setting = -1;
	bool PopupSetKeyIsOpen = false;

	void DrawPopupSetKey();



	enum class ErrorCombination {
		ERROR_ONLY_SYMBOLS_AND_NUMBERS,
		ERROR_TOO_MUCH_COUNT,
		ERROR_COLLISION,
		ERROR_IDENTICAL,
		NOTHING
	};

	ErrorCombination GetErrorOfThisCombination(const std::vector<int>& Keys);

	std::string ErrorCombination_To_string(const ErrorCombination& error);


};




#endif
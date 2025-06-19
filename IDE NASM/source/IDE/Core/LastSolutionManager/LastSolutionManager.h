#ifndef LAST_SOLUTION_MANAGER_H
#define LAST_SOLUTION_MANAGER_H

#include "Utils/File/Utils.File.h"

#include "IDE/Core/SaveSystem/SaveSystem.h"
#include "IDE/Core/Setting/interfaces/ISettingObject.h"


#include "IDE/Core/Solution/SolutionInfo.h"

#include <string>
#include <vector>

class StartupWindow;

class LastSolutionManager : public SaveSystem, public ISettingObject
{
public:
	LastSolutionManager(StartupWindow* startupWindow);
	~LastSolutionManager();


	void AddPath(const std::wstring& path);

	/// <summary>
	/// Рендерить только в Menu
	/// </summary>
	bool Draw();

	std::vector<SolutionInfo>* GetPaths_PTR();


	std::wstring GetChoosedPath();

	void DrawSetting() override;
	nlohmann::json SaveSetting() override;
	void LoadSetting(const nlohmann::json& Data) override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& Data) override;


private:
	StartupWindow* startupWindow = nullptr;

	std::vector<SolutionInfo> LastSolutions;

	int IndexChoosed = 0;

	int PathIsNotEqual(const std::wstring& name);

	bool ShowAllPath = true;
};




#endif
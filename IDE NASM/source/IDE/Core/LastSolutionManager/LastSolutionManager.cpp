#include "LastSolutionManager.h"

#include "IDE/GUI/StartupWindow/StartupWindow.h"

LastSolutionManager::LastSolutionManager(StartupWindow* startupWindow) : SaveSystem(u8"LastSolutions"), ISettingObject(u8"Последние проекты", u8"Общие") {
	this->startupWindow = startupWindow;
		startupWindow->SetPTR_last_solutions(GetPaths_PTR());
}

LastSolutionManager::~LastSolutionManager() {
#ifdef WITH_DEBUG_OUTPUT
	std::cout << "LastPathManager::~LastPathManager()\n";
#endif // !WITH_DEBUG_OUTPUT
}
/// <summary>
/// Возвращает true если пользователь нажал на один из путей
/// </summary>
/// <returns></returns>
bool LastSolutionManager::Draw() {
	bool pressed = false;

	if (LastSolutions.size() == 0)
	{
		ImGui::Text(u8"  Пока что здесь пусто, ну ты это..\nхотя бы сохрани или открой файл я не знаю");
	}
	else {
		for (int i = 0; i < LastSolutions.size(); i++){
			
			std::wstring PathDraw = std::to_wstring(i + 1) + L". ";

			if (!ShowAllPath)
				PathDraw += LastSolutions[i].path2file.substr(LastSolutions[i].path2file.find_last_of(L"\\") + 1);
			else
				PathDraw += LastSolutions[i].path2file;


			std::string str_pathsIsFavorite = wstring_to_stringUTF8(PathDraw);

			if (ImGui::MenuItem(str_pathsIsFavorite.c_str())) {
				IndexChoosed = i;
				pressed = true;
			}

		}
	}
	return pressed;
}

std::wstring LastSolutionManager::GetChoosedPath() {
	return LastSolutions[IndexChoosed].path2file;
}



std::vector<SolutionInfo>* LastSolutionManager::GetPaths_PTR() {
	return &LastSolutions;
}


int LastSolutionManager::PathIsNotEqual(const std::wstring& name) {
	for (int i = 0; i < LastSolutions.size(); i++) {
		if (LastSolutions[i].path2file == name)
			return i;
	}
	return -1;
}

void LastSolutionManager::AddPath(const std::wstring& path) {
	


	if (!isFileExist(path))
		return;

	int index = PathIsNotEqual(path);

	if (index == -1) {
		bool allCorrect = false;
		auto temp = ReadSolutionInfo(path, allCorrect);
		if (allCorrect)
			LastSolutions.emplace_back(temp);
	}
	else {
		std::swap(LastSolutions[index], LastSolutions[0]);
	}
}





void LastSolutionManager::DrawSetting() {
	ISettingObject::DrawBegin();

	if (ImGui::RadioButton(u8"Показывать абсолютный путь", ShowAllPath))
		ShowAllPath = !ShowAllPath;

}




nlohmann::json LastSolutionManager::SaveSetting() {
	nlohmann::json SaveData;

	SaveData["ShowAllPath"] = ShowAllPath;

	return SaveData;
}
void LastSolutionManager::LoadSetting(const nlohmann::json& Data) {
	if (Data.contains("ShowAllPath"))
		ShowAllPath = Data["ShowAllPath"].get<bool>();
}

nlohmann::json LastSolutionManager::Save() {
	nlohmann::json SaveData;

	for (int i = 0; i < LastSolutions.size(); i++) {
		SaveData["Paths"][i]["Path"] = wstring_to_stringUTF8(LastSolutions[i].path2file);
	}

	return SaveData;
}

void LastSolutionManager::Load(const nlohmann::json& Data) {

	if (Data.contains("Paths") == false)
		return;


	for (auto& [index, PathData] : Data["Paths"].items()) {


		std::wstring temp_path;

		if (PathData.contains("Path"))
			temp_path = stringUTF8_to_wstring(PathData["Path"].get<std::string>());
		else
			continue;

		bool allCorrect = false;
		auto temp = ReadSolutionInfo(temp_path, allCorrect);
		if (allCorrect)
			LastSolutions.emplace_back(temp);


	}


	std::sort(
		LastSolutions.begin(),
		LastSolutions.end(),
		[](const SolutionInfo& a, const SolutionInfo& b)
		{
			if (a.isFavorite == b.isFavorite)
				return a.timeLastChange > b.timeLastChange;
			else
				return a.isFavorite > b.isFavorite;

		});



}
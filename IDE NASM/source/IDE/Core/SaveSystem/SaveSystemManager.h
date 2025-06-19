#ifndef SAVE_SYSTEM_MANAGER_H
#define SAVE_SYSTEM_MANAGER_H

#include "SaveSystem.h"

#include "Utils/File/Utils.File.h"
#include "Utils/Text/Utils.Text.h"
#include "Utils/Text/Encoding/Wstring/Utils.Encoding.Wstring.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>


class SaveSystemManager
{
public:
	SaveSystemManager(const std::wstring& SavingPath);
	~SaveSystemManager();

	void ChangePathSaving(const std::wstring& newPath);
	void AddObjectPtr(SaveSystem* object);

	void Save();
	void Load();

private:
	std::wstring SavingPath;
	std::vector<SaveSystem*> objects;

	int GetIndexSaveObject(const std::string& NameObject);

	void ReadFromFile(std::ifstream& FileSave);
	void ReadFromMemory(const std::string& Data);


	void LoadAllSaveToObjects(const nlohmann::json& SaveData);
};

#endif // !SAVE_SYSTEM_MANAGER_H

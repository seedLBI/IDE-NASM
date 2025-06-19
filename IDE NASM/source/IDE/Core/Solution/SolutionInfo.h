#ifndef SOLUTION_INFO_H
#define SOLUTION_INFO_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "Utils/File/Utils.File.h"
#include "Utils/Text/Utils.Text.h"
#include "Utils/Text/Encoding/UTF8/Utils.Encoding.UTF8.h"

struct SolutionInfo {

	std::wstring Path_ProjectAbsolute;

	std::wstring NameFolderProject;
	std::wstring NameFileSolution;


	std::wstring path2file;

	std::string mainFile;
	std::string entryPoint;

	std::string name;
	std::string description;

	uint64_t timeCreated;
	uint64_t timeLastChange;
	uint64_t timeSpent;

	uint64_t countLines;

	nlohmann::json AdditionalSaveData;


	bool isFavorite;

	std::vector<std::string> files;

	std::wstring GetTransliterateNameProject();

	std::wstring GetPathAbsolute();
	std::wstring GetPathSource();
	std::wstring GetPathDebug();
	std::wstring GetPathRelease();

	void UpdateAllFiles();


};


SolutionInfo ReadSolutionInfo(const std::wstring& CompletePath, bool& Status);

nlohmann::json MakeJSONdata(SolutionInfo& data);

void SaveSolution(SolutionInfo& data);

#endif
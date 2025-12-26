#include "SolutionInfo.h"

SolutionInfo ReadSolutionInfo(const std::wstring& CompletePath, bool& Status) {
	SolutionInfo result;
	std::ifstream ifn(CompletePath);


	result.Path_ProjectAbsolute = CompletePath.substr(0, CompletePath.find_last_of(L"\\"));
	result.NameFolderProject = result.Path_ProjectAbsolute.substr(result.Path_ProjectAbsolute.find_last_of(L"\\") + 1);
	result.NameFileSolution = CompletePath.substr(CompletePath.find_last_of(L"\\") + 1);
	result.NameFileSolution = result.NameFileSolution.substr(0, result.NameFileSolution.find_last_of(L"."));


	nlohmann::json solutionData;

	Status = false;

	try {
		solutionData = nlohmann::json::parse(ifn);
	}
	catch (const nlohmann::json::parse_error& e) {
		static_cast<void>(e);
#ifdef _DEBUG
		std::cout << "ERROR  SolutionInfo ReadSolutionInfo(const std::wstring& CompletePath, bool& Status)\n";
#endif // _DEBUG
		return result;
	}

	if (solutionData.contains("name"))
		result.name = solutionData["name"].get<std::string>();
	else
		result.name = "";

	if (solutionData.contains("description"))
		result.description = solutionData["description"].get<std::string>();
	else
		result.description = "";

	if (solutionData.contains("mainFile"))
		result.mainFile = solutionData["mainFile"].get<std::string>();
	else
		result.mainFile = "";

	if (solutionData.contains("entryPoint"))
		result.entryPoint = solutionData["entryPoint"].get<std::string>();
	else
		result.entryPoint = "main";

	if (solutionData.contains("timeCreated"))
		result.timeCreated = solutionData["timeCreated"].get<uint64_t>();
	else
		result.timeCreated = 0;

	if (solutionData.contains("timeLastChange"))
		result.timeLastChange = solutionData["timeLastChange"].get<uint64_t>();
	else
		result.timeLastChange = 0;

	if (solutionData.contains("timeSpent"))
		result.timeSpent = solutionData["timeSpent"].get<uint64_t>();
	else
		result.timeSpent = 0;

	if (solutionData.contains("countLines"))
		result.countLines = solutionData["countLines"].get<uint64_t>();
	else
		result.countLines = 0;

	if (solutionData.contains("isFavorite"))
		result.isFavorite = solutionData["isFavorite"].get<bool>();
	else
		result.isFavorite = false;

	if (solutionData.contains("files")) {
		for (const auto& file : solutionData["files"]) {
			result.files.push_back(file.get<std::string>());
		}
	}

	if (solutionData.contains("AdditionalSaveData")) {
		result.AdditionalSaveData = solutionData["AdditionalSaveData"];
	}

	result.path2file = CompletePath;

	Status = true;

	return result;
}

nlohmann::json MakeJSONdata(SolutionInfo& data) {

	nlohmann::json SaveData;

	data.UpdateAllFiles();

	SaveData["name"] = data.name;
	SaveData["description"] = data.description;
	SaveData["entryPoint"] = data.entryPoint;
	SaveData["mainFile"] = data.mainFile;

	SaveData["timeCreated"] = data.timeCreated;
	SaveData["timeLastChange"] = data.timeLastChange;
	SaveData["timeSpent"] = data.timeSpent;
	SaveData["countLines"] = data.countLines;
	SaveData["isFavorite"] = data.isFavorite;

	SaveData["AdditionalSaveData"] = data.AdditionalSaveData;

	SaveData["files"] = data.files;

	return SaveData;
}

void SaveSolution(SolutionInfo& data) {
	std::ofstream ofn(data.path2file);
	ofn << MakeJSONdata(data).dump(4);
	ofn.close();
}


std::wstring SolutionInfo::GetTransliterateNameProject() {
	return stringUTF8_to_wstring(transliterate_rus(wstring_to_stringUTF8(NameFolderProject)));
}

std::wstring SolutionInfo::GetPathAbsolute() {
	return Path_ProjectAbsolute;
}
std::wstring SolutionInfo::GetPathSource() {
	return L"source";
}
std::wstring SolutionInfo::GetPathDebug() {
	return L"debug";
}
std::wstring SolutionInfo::GetPathRelease() {
	return L"release";
}


void SolutionInfo::UpdateAllFiles() {
	files.clear();

	int Read_countLines = 0;

	std::filesystem::path PathToSource = GetPathAbsolute() + L"\\" + GetPathSource();

	for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(PathToSource)) {

		if (dir_entry.is_directory() == false) {

			std::wstring pathNewFile = dir_entry.path().wstring();

			std::ifstream ifn(pathNewFile);
			Read_countLines += GetCountLinesWithText(ifn);
			ifn.close();



			if (pathNewFile.find(Path_ProjectAbsolute) != std::string::npos) {
				pathNewFile.erase(pathNewFile.begin(), pathNewFile.begin() + Path_ProjectAbsolute.size() + 1);
			}

			files.push_back(wstring_to_stringUTF8(pathNewFile));

		}
	}
	countLines = Read_countLines;

}
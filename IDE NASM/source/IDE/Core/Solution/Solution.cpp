#include "Solution.h"
#include "IDE/Core/LastSolutionManager/LastSolutionManager.h"
#include "IDE/Core/LocalisationManager/LocalisationManager.h"

Solution::Solution() {
#ifdef _DEBUG
	std::cout << "Solution::Solution()\n";
#endif

}

void Solution::Init(WidgetManager_TextEditor* widgetManager_TextEditor, LastSolutionManager* lastSolutionManager,Widget_FilesViewer* widget_FilesViewer, PositionWidgetsManager* positionWidgetsManager, WidgetManager* widgetManager) {
	this->widgetManager_TextEditor = widgetManager_TextEditor;
	this->lastSolutionManager = lastSolutionManager;
	this->widget_FilesViewer = widget_FilesViewer;
	this->positionWidgetsManager = positionWidgetsManager;
	this->widgetManager = widgetManager;
}

SolutionInfo Solution::GetInfo() {
	return CurrentSolution;
}


Solution::~Solution() {
#ifdef _DEBUG
	std::cout << "Solution::~Solution()\n";
#endif
	Close();
}

void Solution::SaveCurrentSolution() {
	if (isOpened) {


		widgetManager_TextEditor->SaveAllFile();

		Timer_spent.stop();
		CurrentSolution.timeSpent += static_cast<uint64_t>(Timer_spent.elapsedSeconds());
		CurrentSolution.timeLastChange = GetCurrentTimestamp();

		CurrentSolution.AdditionalSaveData["Positions"] = positionWidgetsManager->MakeSaveData();
		CurrentSolution.AdditionalSaveData["Widgets"] = widgetManager->Save();


		std::ofstream ofn(CurrentSolution.path2file);
		ofn << MakeJSONdata(CurrentSolution).dump(4);
		ofn.close();


		Timer_spent.start();
	}
}

void Solution::SaveCurrentFocusedFile() {
	widgetManager_TextEditor->SaveCurrentActiveFile();
}

void Solution::SaveAllFiles() {
	widgetManager_TextEditor->SaveAllFile();
	SaveCurrentSolution();
}


void Solution::DrawPopupCreation() {
	static const std::wstring PathRepos = GetPathDirectory() + L"\\repos";

	auto ClearArray = [](char* input_Name, const int& size) {
		for (int i = 0; i < size; i++)
			input_Name[i] = 0;
		};

	static bool firstTime = true;
	static char input_Name[256];
	static char input_Path[2048];
	static char input_Discription[2048];

	static std::string BasePath = wstring_to_stringUTF8(PathRepos);

	if (firstTime) {
		firstTime = false;
		ClearArray(input_Name, 256);

		ClearArray(input_Discription, 2048);
	}


	static const ImGuiWindowFlags flagsWindow =
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImVec2 size = ImGui::GetMainViewport()->WorkSize;

	float HeightWindow = ImGui::GetTextLineHeight() * 19.5f;

	

	if (flag_PopupCreation) {


		ImGui::SetNextWindowSize(ImVec2(ImGui::CalcTextSize(u8"                                                                                 ").x, HeightWindow));


		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));


		ImGui::OpenPopup("###CREATION_NEW_PROJECT");



		if (ImGui::BeginPopupModal((tr("solution.title") + " ###CREATION_NEW_PROJECT").c_str(), &flag_PopupCreation, flagsWindow)) {


			center.x -= ImGui::GetCurrentWindow()->Size.x / 2;
			center.y -= ImGui::GetCurrentWindow()->Size.y / 2;
			ImGui::GetCurrentWindow()->Pos = center;
			
			
			ImGui::Text(tr("solution.nameProject").c_str());
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 52.f);
			ImGui::InputText(u8"###INPUT_NAME_PROJECT", input_Name, 1024, ImGuiInputTextFlags_NoLabel);
			
			
			std::string PathToProject = BasePath + "\\" + input_Name;
			
			
			ClearArray(input_Path, 2048);
			for (int i = 0; i < PathToProject.size(); i++)
			{
				if (i > 2042) {
					break;
				}
				input_Path[i] = PathToProject[i];
			}


			ImGui::Text(tr("solution.pathProject").c_str());
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 52.f);
			ImGui::InputText(u8"###INPUT_PATH_NEW_PROJECT", input_Path, 2048, ImGuiInputTextFlags_NoLabel | ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			if (ImGui::Button(u8"...##BUTTONCHANGEPATH", ImVec2(0, 0))) {
				std::wstring NewBasePath = Open_FolderDialog(stringUTF8_to_wstring(tr("solution.dialogChoosePathFolder.title")).c_str(), PathRepos.c_str());

				if (NewBasePath.empty() == false) {
					BasePath = wstring_to_stringUTF8(NewBasePath);
				}

			}
			TooltipTranslated("solution.button.change.tooltip");
			

			ImGui::Text(tr("solution.description").c_str()); ImGui::SameLine(); HelpMarker(tr("solution.description.helpMarker").c_str());
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 52.f);
			ImGui::InputTextMultiline(u8"###INPUT_DESCRIPTION_NEW_PROJECT", input_Discription, 2048,ImVec2(0,0), ImGuiInputTextFlags_NoLabel);

			ImGui::Dummy(ImVec2(0,ImGui::GetTextLineHeight()*0.7f));


			bool disabled = std::string(input_Name).empty();

			
			if (ButtonCenteredOnLine((" " + tr("solution.button.create") + " ").c_str(), 1.0f)) {
				if (disabled == false) {

					std::string str_Name = input_Name;
					std::string str_Discription = input_Discription;
					std::string str_Path = PathToProject + "\\" + str_Name + ".nasmsln";



					Close();



					CreateFolder(stringUTF8_to_wstring(PathToProject));
					CreateFolder(stringUTF8_to_wstring(PathToProject + "\\source"));
					CreateFolder(stringUTF8_to_wstring(PathToProject + "\\x86"));
					CreateFolder(stringUTF8_to_wstring(PathToProject + "\\x64"));
					CreateFolder(stringUTF8_to_wstring(PathToProject + "\\x86\\debug"));
					CreateFolder(stringUTF8_to_wstring(PathToProject + "\\x86\\release"));
					CreateFolder(stringUTF8_to_wstring(PathToProject + "\\x64\\debug"));
					CreateFolder(stringUTF8_to_wstring(PathToProject + "\\x64\\release"));

					std::wstring PathToMainFile = stringUTF8_to_wstring(PathToProject + "\\source\\main.asm");
					std::wstring RootPathApplication = GetPathDirectory();

					std::ofstream ofn(PathToMainFile);
					ofn << TemplateDefaultMainFile;
					ofn.close();

					std::ifstream ifn(PathToMainFile);
					int countLinesInMainFile = GetCountLinesWithText(ifn);
					ifn.close();


					std::wstring wstr_Base = stringUTF8_to_wstring(BasePath + "\\");


					PathToMainFile = L"source\\main.asm";


					SolutionInfo newSolution;

					newSolution.isFavorite = false;
					newSolution.name = str_Name;
					newSolution.path2file = stringUTF8_to_wstring(str_Path);
					newSolution.description = str_Discription;
					newSolution.entryPoint = "main";
					newSolution.mainFile = wstring_to_stringUTF8(PathToMainFile);
					newSolution.timeSpent = 0;
					newSolution.timeCreated = GetCurrentTimestamp();
					newSolution.timeLastChange = newSolution.timeCreated;
					newSolution.files.push_back(wstring_to_stringUTF8(PathToMainFile));
					newSolution.countLines = countLinesInMainFile;


					std::wstring AbsolutePathToFileSolution = stringUTF8_to_wstring(str_Path);

					newSolution.Path_ProjectAbsolute = AbsolutePathToFileSolution.substr(0, AbsolutePathToFileSolution.find_last_of(L"\\"));
					newSolution.NameFolderProject = newSolution.Path_ProjectAbsolute.substr(newSolution.Path_ProjectAbsolute.find_last_of(L"\\") + 1);
					newSolution.NameFileSolution = AbsolutePathToFileSolution.substr(AbsolutePathToFileSolution.find_last_of(L"\\") + 1);
					newSolution.NameFileSolution = newSolution.NameFileSolution.substr(0, newSolution.NameFileSolution.find_last_of(L"."));


					SaveSolution(newSolution);


					OpenFromPath(newSolution.path2file);

					positionWidgetsManager->SetupDefaultLayout();
					positionWidgetsManager->SetFirstTextEditor(widgetManager_TextEditor->GetWidgets().front()->GetName());
					widgetManager_TextEditor->GetWidgets().front()->SetFlagShow(true);
					widgetManager_TextEditor->GetWidgets().front()->MakeActiveCurrentWidget();

					flag_isCreated = true;

					ClearArray(input_Name, 256);
					ClearArray(input_Discription, 2048);
					BasePath = wstring_to_stringUTF8(PathRepos);
					flag_PopupCreation = false;
				}

			}
			


			ImGui::EndPopup();
		}


		//ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}
	


}

void Solution::Create() {

	if (!ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel)) {
		flag_PopupCreation = true;
	}

}
bool Solution::isCreated() {
	bool temp = flag_isCreated;
	flag_isCreated = false;
	return temp;
}


void Solution::Close() {
#ifdef _DEBUG
	std::cout << "void Solution::Close()\n";
#endif
	if (isOpened) {
		SaveCurrentSolution();
	}

	isOpened = false;
}

bool Solution::Open() {

	const std::wstring PathRepos = GetPathDirectory() + L"\\repos";

	std::wstring path = Open_FileDialog(stringUTF8_to_wstring(tr("solution.dialogOpenFile.title")).c_str(), L"NASM Solution (*.nasmsln)\0*.nasmsln\0\0", PathRepos.c_str());


	if (path.empty())
		return false;

	return OpenFromPath(path);
}

bool Solution::OpenFromPath(const std::wstring& CompletePath) {

	bool SuccessReading = true;
	SolutionInfo tempSolution = ReadSolutionInfo(CompletePath, SuccessReading);

	if (SuccessReading) {

		if (isOpened) {
			Close();
		}

		lastSolutionManager->AddPath(CompletePath);

		CurrentSolution = tempSolution;

		widget_FilesViewer->LoadSolutionInfo(&CurrentSolution);
		widgetManager_TextEditor->LoadSolutionInfo(&CurrentSolution);


		isOpened = true;

		if (tempSolution.AdditionalSaveData.contains("Positions"))
			positionWidgetsManager->LoadSaveData(tempSolution.AdditionalSaveData["Positions"].get<std::string>());

		if (tempSolution.AdditionalSaveData.contains("Widgets"))
			widgetManager->Load(tempSolution.AdditionalSaveData["Widgets"]);

		Timer_spent.start();

		return true;
	}
	else {
		return false;
	}

}
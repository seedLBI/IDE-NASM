#include "MainMenuBar.h"



bool MainMenuBar::IsMouseOverMenuBar() {
	ImVec2 mousePos = ImGui::GetMousePos();
	return mousePos.x >= menuBarPos.x && mousePos.x <= menuBarPos.x + menuBarSize.x &&
		mousePos.y >= menuBarPos.y && mousePos.y <= menuBarPos.y + menuBarSize.y;
}

void MainMenuBar::DrawIcon() {
	const float frameHeight = ImGui::GetFrameHeight();
	ImGui::SetCursorPosX(ImGui::GetStyle().ItemSpacing.x * 0.5f);
	ImGui::Image((ImTextureID)icon_logo.GetID(), ImVec2(frameHeight, frameHeight), ImVec2(0, 1), ImVec2(1, 0), color_Logo, ImVec4(0,0,0,0));
	TooltipTranslated("menu.logo.tooltip");
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.x);
}



int MainMenuBar::GetHeightMenu() {
	return heightMenu;
}

void MainMenuBar::DrawProjectName() {
	const std::string projectName = wstring_to_stringUTF8(solution->GetInfo().NameFolderProject);

	ImVec2 padding = ImGui::GetStyle().FramePadding;
	ImVec2 textSize = ImGui::CalcTextSize(projectName.c_str());
	float totalW = textSize.x + padding.x * 2.0f;
	float totalH = textSize.y + padding.y * 2.0f;

	float shrinkFactor = 0.8f;
	float newH = totalH * shrinkFactor;

	float frameH = ImGui::GetFrameHeight();

	float vertOffset = (frameH - newH) * 0.5f;

	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.x + 10.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + vertOffset);

	ImGui::Dummy(ImVec2(totalW, newH));

	TooltipTranslated("menu.projectName");



	ImVec2 bb_min = ImGui::GetItemRectMin();
	ImVec2 bb_max = ImGui::GetItemRectMax();

	float textOffsetY = (newH - textSize.y) * 0.5f;

	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImU32 col_text = ImGui::GetColorU32(ImGuiCol_Text);

	draw->AddRectFilled(ImVec2(bb_min.x, bb_min.y + 3.f) , bb_max, color_ProjectBackground, 0.0f);
	draw->AddText(
		ImVec2(bb_min.x + padding.x,
			bb_min.y + textOffsetY),
		col_text,
		projectName.c_str()
	);
}

void MainMenuBar::DrawTitleButtons() {
	ImGuiStyle& style = ImGui::GetStyle();
	float windowWidth = ImGui::GetWindowWidth();
	float buttonWidth = 40.0f;
	float spacing = style.ItemSpacing.x;
	float paddingRight = style.WindowPadding.x;
	float totalWidth = 3 * buttonWidth;

	ImGui::SetCursorPosX(windowWidth - totalWidth);

	ImVec4 colBtn = style.Colors[ImGuiCol_Button];
	ImVec4 colBtnHover = style.Colors[ImGuiCol_ButtonHovered];
	ImVec4 colBtnActive = style.Colors[ImGuiCol_ButtonActive];

	ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_MenuBarBg]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colBtnHover);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, colBtnActive);

	const ImVec2 buttonSize = ImVec2(buttonWidth, ImGui::GetFrameHeight() - 1.f);


	if (ImGui::Button(ICON_FA_MINUS, buttonSize)) {
		glfwIconifyWindow(window);
	}
	titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
	TooltipTranslated("menu.button.hide.tooltip");


	ImGui::SameLine(0,0);

	
	if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
		if (ImGui::Button(ICON_FA_COMPRESS, buttonSize)) {
			glfwRestoreWindow(window);
		}
		titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
		TooltipTranslated("menu.button.collapse.tooltip");
	}
	else {
		if (ImGui::Button(ICON_FA_EXPAND, buttonSize)) {
			glfwMaximizeWindow(window);
		}
		titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
		TooltipTranslated("menu.button.expand.tooltip");
	}
	

	ImGui::SameLine(0,0);


	if (ImGui::Button(ICON_FA_XMARK, buttonSize)) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
	TooltipTranslated("menu.button.close.tooltip");

	ImGui::PopStyleColor(3);
}

void MainMenuBar::SetFlag_DrawAll() {
	DrawingAllButtons = true;
}


bool MainMenuBar::IsPointOverTitleButton(const POINT& pt) const {
	for (const auto& r : titleButtonRects) {
		if (pt.x >= r.Min.x && pt.x < r.Max.x &&
			pt.y >= r.Min.y && pt.y < r.Max.y)
			return true;
	}
	return false;
}

void MainMenuBar::Draw() {

	heightMenu = 0.f;

	std::string MenuItem_File		= std::string(ICON_FA_FILE)				+ " " + tr("menu.item.file");
	std::string MenuItem_View		= std::string(ICON_FA_MOUNTAIN_SUN)		+ " " + tr("menu.item.view");
	std::string MenuItem_Setting	= std::string(ICON_FA_GEAR)				+ " " + tr("menu.item.settings");
	std::string MenuItem_Assembly	= std::string(ICON_FA_HAMMER)			+ " " + tr("menu.item.build");
	std::string MenuItem_Debug		= std::string(ICON_FA_BUG)				+ " " + tr("menu.item.debug");
	std::string MenuItem_About		= std::string(ICON_FA_LIFE_RING)		+ " " + tr("menu.item.about");
	
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 0.f));

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 7));

	if (ImGui::BeginMainMenuBar()) {

		float height = ImGui::GetFrameHeight();
		heightMenu += height;

		titleButtonRects.clear();

		menuBarPos = ImGui::GetWindowPos();
		menuBarSize = ImGui::GetWindowSize();


		DrawIcon();


		if (DrawingAllButtons)
		{

			if (ImGui::BeginMenu(MenuItem_File.c_str())) {

				std::string MenuItem_FileItem_CreateProject = tr("menu.item.file.createProject");
				std::string MenuItem_FileItem_OpenProject	= tr("menu.item.file.openProject");
				std::string MenuItem_FileItem_SaveProject	= tr("menu.item.file.saveProject");
				std::string MenuItem_FileItem_LastProjects	= tr("menu.item.file.lastProjects");
				std::string MenuItem_FileItem_Exit			= tr("menu.item.file.exit");

				if (ImGui::MenuItem(MenuItem_FileItem_CreateProject.c_str())) {
					solution->Create();
				}
				if (ImGui::MenuItem(MenuItem_FileItem_OpenProject.c_str())) {
					if (solution->Open()) {
						buildManager->ClearOutput();
					}
				}
				if (ImGui::MenuItem(MenuItem_FileItem_SaveProject.c_str())) {
					solution->SaveCurrentSolution();
				}
				if (ImGui::BeginMenu(MenuItem_FileItem_LastProjects.c_str())) {

					if (lastSolutionManager->Draw()) {
						solution->OpenFromPath(lastSolutionManager->GetChoosedPath());
					}

					ImGui::EndMenu();
				}

				ImGui::MenuItem(MenuItem_FileItem_Exit.c_str());


				ImGui::EndMenu();
			}
			titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));



			if (ImGui::BeginMenu(MenuItem_View.c_str())) {
				
				std::string MenuItem_ViewWidget_FilesViewer =	std::string(ICON_FA_FOLDER_TREE) + " " + tr("menu.item.view.filesViewer");
				std::string MenuItem_ViewWidget_Output =		std::string(ICON_FA_FOLDER_TREE) + " " + tr("menu.item.view.output");


				ImGui::MenuItem(MenuItem_ViewWidget_FilesViewer.c_str(), "", widgetManager->GetWidgetPtrByName(u8"Список файлов")->GetPtrFlagShow());
				ImGui::MenuItem(MenuItem_ViewWidget_Output.c_str(),      "", widgetManager->GetWidgetPtrByName(u8"Вывод")->GetPtrFlagShow());



				ImGui::EndMenu();
			}
			titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));



			if (ImGui::BeginMenu(MenuItem_Assembly.c_str())) {

				std::string MenuItem_BuildItem_build_And_run	= tr("menu.item.build.run");
				std::string MenuItem_BuildItem_compile_And_link = tr("menu.item.build.compileLink");
				std::string MenuItem_BuildItem_compile			= tr("menu.item.build.compile");

				if (ImGui::MenuItem(MenuItem_BuildItem_build_And_run.c_str())) {
					buildManager->Run();
				}
				if (ImGui::MenuItem(MenuItem_BuildItem_compile_And_link.c_str())) {
					buildManager->CompileAndLink();
				}
				if (ImGui::MenuItem(MenuItem_BuildItem_compile.c_str())) {
					buildManager->Compile();
				}

				ImGui::EndMenu();
			}
			titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));



			if (ImGui::MenuItem(MenuItem_Setting.c_str())) {
				setting->Open();
			}
			titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));



			if (ImGui::MenuItem(MenuItem_About.c_str())) {
				
			}
			titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));



			DrawProjectName();

		}

		DrawTitleButtons();

		ImGui::EndMainMenuBar();
	}

	ImGui::PopStyleVar();


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	float height = ImGui::GetFrameHeight();

	if (DrawingAllButtons)
	{
		if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", NULL, ImGuiDir_Up, height, window_flags)) {
			if (ImGui::BeginMenuBar()) {

				float height = ImGui::GetFrameHeight();
				heightMenu += height;

				ImGui::Dummy(ImVec2(20.f, 0.f));

				ImGuiStyle& style = ImGui::GetStyle();
				ImVec4 colBtn = style.Colors[ImGuiCol_Button];
				ImVec4 colBtnHover = style.Colors[ImGuiCol_ButtonHovered];
				ImVec4 colBtnActive = style.Colors[ImGuiCol_ButtonActive];

				ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_MenuBarBg]);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colBtnHover);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, colBtnActive);

				if (ImGui::Button(ICON_FA_FILE_CIRCLE_PLUS, ImVec2(55.f, 0.f))) {
					solution->Create();
				}
				titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
				TooltipTranslated("menu.button.createProject.tooltip");


				ImGui::SameLine(0, 0);


				if (ImGui::Button(ICON_FA_FOLDER_OPEN, ImVec2(55.f, 0.f))) {
					if (solution->Open()) {
						buildManager->ClearOutput();
					}
				}
				titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
				TooltipTranslated("menu.button.openProject.tooltip");


				ImGui::SameLine(0, 0);


				if (ImGui::Button(ICON_FA_FLOPPY_DISK, ImVec2(55.f, 0.f))) {
					solution->SaveCurrentSolution();
				}
				titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
				TooltipTranslated("menu.button.saveProject.tooltip");




				ImGui::PopStyleColor(3);

				auto& settings = buildManager->GetCurrentSettings();

				const char* str_Release = "Release";
				const char* str_Debug = "Debug";

				
				std::string CurrentPlatform = "";

				if (settings.platform == BuildManager_DefaultPlatforms::Release)
					CurrentPlatform = str_Release;
				else if (settings.platform == BuildManager_DefaultPlatforms::Debug)
					CurrentPlatform = str_Debug;



				ImGui::SetNextItemWidth(ImGui::CalcTextSize("RELEASE       ").x);


				ImGui::BeginDisabled(buildManager->GetState() != BuildManager_Free);

				if (ImGui::BeginCombo(u8"##SELECT_PLATFORM", CurrentPlatform.c_str())) {

					if (ImGui::Selectable(str_Release)) {
						buildManager->SetPlatform(BuildManager_DefaultPlatforms::Release);
					}
					if (CurrentPlatform == str_Release)
						ImGui::SetItemDefaultFocus();

					if (ImGui::Selectable(str_Debug)) {
						buildManager->SetPlatform(BuildManager_DefaultPlatforms::Debug);
					}
					if (CurrentPlatform == str_Debug)
						ImGui::SetItemDefaultFocus();


					ImGui::EndCombo();
				}
				titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
				TooltipTranslated("menu.selectConfig.tooltip");


				ImGui::EndDisabled();

				const char* str_64 = "x64";
				const char* str_32 = "x86";

				std::string CurrentArch = "";

				if (settings.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows32)
					CurrentArch = str_32;
				else if (settings.arg_architecture == Compiler_ARGS_ARCHITECTURE::Windows64)
					CurrentArch = str_64;


				ImGui::SetNextItemWidth(ImGui::CalcTextSize("RELEASE ").x);

				ImGui::BeginDisabled(buildManager->GetState() != BuildManager_Free);

				if (ImGui::BeginCombo(u8"##SELECT_ARCH", CurrentArch.c_str())) {

					if (ImGui::Selectable(str_32)) {
						buildManager->SetArch(Compiler_ARGS_ARCHITECTURE::Windows32);
					}
					if (CurrentArch == str_32)
						ImGui::SetItemDefaultFocus();

					if (ImGui::Selectable(str_64)) {
						buildManager->SetArch(Compiler_ARGS_ARCHITECTURE::Windows64);
					}
					if (CurrentArch == str_64)
						ImGui::SetItemDefaultFocus();

					ImGui::EndCombo();
				}
				titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
				TooltipTranslated("menu.selectArch.tooltip");


				ImGui::EndDisabled();


				static bool firstTime = true;
				static char buf_entryPoint[256];

				if (firstTime) {

					for (int i = 0; i < 256; i++) {
						buf_entryPoint[i] = 0;
					}
					
					for (int i = 0; i < settings.str_entryPoint.size(); i++) {
						if (i < 256) {
							buf_entryPoint[i] = settings.str_entryPoint[i];
						}
					}

					firstTime = false;

				}

				ImGui::Text(tr("menu.entryPoint").c_str());
				ImGui::SetNextItemWidth(ImGui::CalcTextSize(u8"Точка входа    ").x);

				ImGui::BeginDisabled(buildManager->GetState() != BuildManager_Free);

				if (ImGui::InputText(u8"##ENTRYPOINTINPUT", buf_entryPoint, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
					firstTime = true;
					buildManager->SetEntryPoint(buf_entryPoint);
				}
				titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
				TooltipTranslated("menu.entryPoint.tooltip");


				ImGui::EndDisabled();


				ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_MenuBarBg]);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colBtnHover);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, colBtnActive);



				std::string MenuQuick_Terminate = std::string(ICON_FA_STOP) + " " + tr("menu.quick.terminate");
				std::string MenuQuick_Run		= std::string(ICON_FA_PLAY) + " " + tr("menu.quick.run");
				std::string MenuQuick_Debug		= std::string(ICON_FA_PLAY) + " " + tr("menu.quick.debug");


				if (buildManager->GetState() == BuildManager_States::BuildManager_Running) {
					if (ImGui::Button(MenuQuick_Terminate.c_str())) {
						buildManager->TerminateRun();
					}
					titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
				}
				else {
					ImGui::BeginDisabled(buildManager->GetState() != BuildManager_Free);

					if (ImGui::Button(MenuQuick_Run.c_str())) {
						buildManager->Run();
					}
					titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));

					ImGui::EndDisabled();
				}

				ImGui::BeginDisabled(buildManager->GetState() != BuildManager_Free);
					ImGui::Button(MenuQuick_Debug.c_str());
					titleButtonRects.push_back(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
				ImGui::EndDisabled();

				ImGui::PopStyleColor(3);



				ImGui::EndMenuBar();
			}
		}
		ImGui::End();
	}

	ImGui::PopStyleColor();






	if (DrawingAllButtons)
	{
		if (ImGui::BeginViewportSideBar("##MainStatusBar", NULL, ImGuiDir_Down, height, window_flags)) {
			if (ImGui::BeginMenuBar()) {


				ImGui::Text(tr("menu.statusBar.nothing").c_str());


				ImGui::EndMenuBar();
			}
		}
		ImGui::End();
	}

}

void MainMenuBar::Update() {

}


void MainMenuBar::LoadColors() {
	std::string toSearch;

	for (int i = 0; i < object_colors.colors.size(); i++) {

		toSearch = object_colors.colors[i].nameColor;

		if (toSearch == u8"color.menu.backgroundProjectName")
			color_ProjectBackground = object_colors.colors[i].color;
		else if (toSearch == u8"color.menu.logo")
			color_Logo = object_colors.colors[i].color;
	}
}
std::vector<NamedColor> MainMenuBar::GetDefaultLightColors() {
	std::vector<NamedColor> colors = {
		{ u8"color.menu.backgroundProjectName",	ImColor(210,186,151, 255)},
		{ u8"color.menu.logo",					ImColor(255 - 94,255 - 157, 255 - 251, 255) }
	};

	return colors;
}
std::vector<NamedColor> MainMenuBar::GetDefaultDarkColors() {
	std::vector<NamedColor> colors = {
		{u8"color.menu.backgroundProjectName",	ImColor(15,15,15,255)},
		{u8"color.menu.logo",					ImColor(94, 157, 251, 255)}
	};

	return colors;
}



MainMenuBar::MainMenuBar(
	WindowManager* windowManager, 
	WidgetManager* widgetManager, 
	LastSolutionManager* lastSolutionManager, 
	Solution* solution, 
	Setting* setting, 
	BuildManager* buildManager) : IThemeLoadable(u8"menu") {

	this->windowManager = windowManager;
	this->widgetManager = widgetManager;
	this->lastSolutionManager = lastSolutionManager;
	this->solution = solution;
	this->setting = setting;
	this->buildManager = buildManager;

	window = windowManager->GetMainWindow()->GetHandle();

	LoadIcon();

	IThemeLoadable::InitListWord(
		{
			u8"color.menu.backgroundProjectName", 
			u8"color.menu.logo"
		}
	);


}
void MainMenuBar::LoadIcon() {

	if (isFileExist("resources\\icons\\LOGO_transparent.png")) {
		TextureSetting settingFlags;
		settingFlags.Min = TextureFilter::LINEAR;
		settingFlags.Max = TextureFilter::LINEAR_MIPMAP_LINEAR;
		settingFlags.WrapX = TextureWrap::CLAMP_TO_EDGE;
		settingFlags.WrapY = TextureWrap::CLAMP_TO_EDGE;
		icon_logo.SetSetting(settingFlags);
		icon_logo.LoadTexture("resources\\icons\\LOGO_transparent.png");
		icon_logo.Init();
	}
	else {

		std::wstring title = stringUTF8_to_wstring(tr("error.menu.loadLogo.title"));
		std::wstring body = stringUTF8_to_wstring(tr("error.menu.loadLogo")) + L" (resources\\icons\\LOGO_transparent.png)";


		MessageBoxW(glfwGetWin32Window(window), body.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
		std::abort();
	}
}
MainMenuBar::~MainMenuBar() {

}
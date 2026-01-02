#include "Application.h"



Application::Application() {

	if (HaveFolder(GetPathDirectory() + L"\\repos") == false) {
		CreateFolder(GetPathDirectory() + L"\\repos");
	}

	InitOpenglWindow();
	Init_IconApplication();
	Init_BorderWindow();
	Init_ImGui();

	fontManager = new FontManager(windowManager->GetMainWindow()->GetHandle());

	fontManager->ReloadFont();

	fps_limiter = new FPS_Timer;
	fps_limiter->SetTargetFPS(60);

	widgetManager = new WidgetManager(fps_limiter);

#ifdef _DEBUG
	std::wcout << GetPathDirectory() + L"\\" + Path_SaveData << std::endl;
#endif // !_DEBUG

	saveSystemManager = new SaveSystemManager(GetPathDirectory() + L"\\" + Path_SaveData);


	outlineStatus = new OutlineStatus(fps_limiter, windowManager);


	setting = new Setting;
	solution = new Solution;
	buildManager = new BuildManager(solution, outlineStatus);
	themeManager = new ThemeManager;
	baseColors = new BaseColors;

	widgetManager_TextEditor = new WidgetManager_TextEditor(fps_limiter);
	widget_FilesViewer = new Widget_FilesViewer(widgetManager_TextEditor);
	widget_OutputConsole = new Widget_OutputConsole(windowManager->GetMainWindow()->GetHandle(), buildManager, solution, fontManager, widgetManager_TextEditor);

	startupWindow = new StartupWindow(fontManager, solution);

	positionWidgetsManager = new PositionWidgetsManager(widgetManager);

	lastSolutionManager = new LastSolutionManager(startupWindow);
	mainMenuBar = new MainMenuBar(windowManager, widgetManager,lastSolutionManager,solution,setting, buildManager);

	notificationManager = new NotificationManager(windowManager->GetMainWindow(), fps_limiter);
	keyCombinationHandler = new KeyCombinationHandler(notificationManager,fps_limiter, windowManager->GetMainWindow());


	solution->Init(widgetManager_TextEditor, lastSolutionManager, widget_FilesViewer, positionWidgetsManager, widgetManager);






	*widget_FilesViewer->GetPtrFlagShow() = true;
	*widget_OutputConsole->GetPtrFlagShow() = true;
	*widgetManager_TextEditor->GetPtrFlagShow() = true;

	InitSaveSystemManager();
	InitSetting();
	InitWidgetManager();
	InitKeyCombinationHandler();
	InitThemeManager();


	saveSystemManager->Load();
}

void Application::InitThemeManager() {
	themeManager->AddObject(baseColors);
	themeManager->AddObject(widgetManager_TextEditor);
	themeManager->AddObject(&HighlighterSyntax::Instance());
	themeManager->AddObject(widget_OutputConsole);
	themeManager->AddObject(outlineStatus);
	themeManager->AddObject(mainMenuBar);
	
}
void Application::InitSaveSystemManager() {
	saveSystemManager->AddObjectPtr(setting);
	saveSystemManager->AddObjectPtr(lastSolutionManager);
}
void Application::InitSetting() {
	setting->AddSettingObject(lastSolutionManager);
	setting->AddSettingObject(&LocalisationManager::getInstance());
	setting->AddSettingObject(keyCombinationHandler);
	setting->AddSettingObject(themeManager);
}
void Application::InitWidgetManager() {
	widgetManager->AddWidgetPtr(widget_FilesViewer);
	widgetManager->AddWidgetPtr(widget_OutputConsole);
	widgetManager->AddWidgetPtr(widgetManager_TextEditor);
}
void Application::InitKeyCombinationHandler() {
	keyCombinationHandler->AddCombination(
		"keyCombination.saveCurrentFocusedFile",
		KeyCombination({ 
			GLFW_KEY_LEFT_CONTROL, 
			GLFW_KEY_S }, 
		std::bind(&Solution::SaveCurrentFocusedFile, solution))
	);

	keyCombinationHandler->AddCombination(
		"keyCombination.saveAllFiles",
		KeyCombination({ 
			GLFW_KEY_LEFT_CONTROL, 
			GLFW_KEY_LEFT_SHIFT,
			GLFW_KEY_S },
		std::bind(&Solution::SaveAllFiles, solution))
	);

	keyCombinationHandler->AddCombination(
		"keyCombination.openNewProject",
		KeyCombination({ 
			GLFW_KEY_LEFT_CONTROL, 
			GLFW_KEY_O },
			std::bind(&Solution::Open, solution))
	);

	keyCombinationHandler->AddCombination(
		"keyCombination.compile",
		KeyCombination({
			GLFW_KEY_LEFT_CONTROL,
			GLFW_KEY_F7 },
			std::bind(&BuildManager::Compile, buildManager))
	);

	keyCombinationHandler->AddCombination(
		"keyCombination.build",
		KeyCombination({
			GLFW_KEY_F7 },
			std::bind(&BuildManager::CompileAndLink, buildManager))
	);

	keyCombinationHandler->AddCombination(
		"keyCombination.run",
		KeyCombination({
			GLFW_KEY_LEFT_CONTROL,
			GLFW_KEY_F5 },
			std::bind(&BuildManager::Run, buildManager))
	);



	keyCombinationHandler->AddCombination(
		"keyCombination.nextLanguage",
		KeyCombination(
			{},
			std::bind(&LocalisationManager::SetNextLanguage, &LocalisationManager::getInstance()))
	);

	keyCombinationHandler->AddCombination(
		"keyCombination.prevLanguage",
		KeyCombination(
			{},
			std::bind(&LocalisationManager::SetPrevLanguage, &LocalisationManager::getInstance()))
	);

	keyCombinationHandler->AddCombination(
		"keyCombination.prevTheme",
		KeyCombination(
			{},
			std::bind(&ThemeManager::SetPrevTheme, themeManager))
	);

	keyCombinationHandler->AddCombination(
		"keyCombination.nextTheme",
		KeyCombination(
			{},
			std::bind(&ThemeManager::SetNextTheme, themeManager))
	);

}


void Application::Draw() {

	outlineStatus->Draw();

	solution->DrawPopupCreation();
	mainMenuBar->Draw();

	widgetManager->Draw();
	setting->Draw();
	notificationManager->Draw();

#ifdef _DEBUG
	ImGui::ShowDemoWindow();
#endif


}


void Application::Update() {
	notificationManager->Update();
	keyCombinationHandler->Update();
	widgetManager->Update();
	outlineStatus->Update();
}


Application::~Application() {
#ifdef _DEBUG
	std::cout << "ENTER -> Application::~Application()\n";
#endif

	solution->~Solution();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	fontManager->~FontManager();
	ImGui::DestroyContext();

	windowManager->~WindowManager();

	glfwTerminate();


	saveSystemManager->Save();



#ifdef _DEBUG
	std::cout << "LEAVE -> Application::~Application()\n";
#endif
}







void Application::Run() {

	outlineStatus->EnableMode_sinAlpha();

	while (windowManager->GetMainWindow()->IsShouldClose() == false && startupWindow->NeedClose() == false) {
		BeginDraw();

			mainMenuBar->Draw();
			outlineStatus->Draw();
			solution->DrawPopupCreation();
			startupWindow->Draw();
#ifdef _DEBUG
			ImGui::ShowDemoWindow();
#endif
		EndDraw();

		outlineStatus->Update();
	}
	outlineStatus->DisableMode_sinAlpha();



	mainMenuBar->SetFlag_DrawAll();

	while (windowManager->GetMainWindow()->IsShouldClose() == false) {

		BeginDraw();
			Draw();
		EndDraw();

		Update();

	}

}

void Application::BeginDraw() {
	glfwPollEvents();


	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(BufferBit::COLOR_BUFFER_BIT | BufferBit::DEPTH_BUFFER_BIT);

	fontManager->ReloadFont();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	positionWidgetsManager->SetDockspaceID(ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode));

}
void Application::EndDraw() {;

	ImGui::EndFrame();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(windowManager->GetMainWindow()->GetHandle());

	fps_limiter->wait();
}



MainMenuBar* Application::GetPTR_MainMenuBar() {
	return mainMenuBar;
}



LRESULT CALLBACK custom_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	auto it = g_hwnd_to_window.find(hwnd);
	GLFWwindow* win = it->second;
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(win));

	switch (msg) {
	case WM_NCCALCSIZE: {
		auto params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

		if (IsZoomed(hwnd)) {
			HMONITOR hMon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi = { sizeof(mi) };
			GetMonitorInfo(hMon, &mi);
			RECT rcWork = mi.rcWork;

			params->rgrc[0].left = rcWork.left;
			params->rgrc[0].top = rcWork.top;
			params->rgrc[0].right = rcWork.right;
			params->rgrc[0].bottom = rcWork.bottom;

			return WVR_REDRAW;
		}
		else {
			return 0;
		}
		break;
	}
	case WM_NCHITTEST: {

		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		ScreenToClient(hwnd, &pt);

		RECT rc;
		GetClientRect(hwnd, &rc);
		const int border = 4;

		if (!IsZoomed(hwnd) && glfwGetWindowMonitor(win) == NULL) {
			bool left = pt.x < border;
			bool right = pt.x >= rc.right - border;
			bool top = pt.y < border;
			bool bottom = pt.y >= rc.bottom - border;

			if (left && top)     return HTTOPLEFT;
			if (right && top)    return HTTOPRIGHT;
			if (left && bottom)  return HTBOTTOMLEFT;
			if (right && bottom) return HTBOTTOMRIGHT;

			if (left)   return HTLEFT;
			if (right)  return HTRIGHT;
			if (top)    return HTTOP;
			if (bottom) return HTBOTTOM;
		}




		float menuHeight = (float)app->GetPTR_MainMenuBar()->GetHeightMenu();
		if (pt.y >= 0 && pt.y < menuHeight) {
			if (app->GetPTR_MainMenuBar()->IsPointOverTitleButton(pt)) {
				return HTCLIENT;
			}
			if (glfwGetWindowMonitor(win) == NULL)
				return HTCAPTION;
		}

		return HTCLIENT;
	}
	case WM_SETCURSOR: {

		if (IsZoomed(hwnd) || glfwGetWindowMonitor(win)) {
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return TRUE;
		}

		int hitTest = LOWORD(lParam);
		switch (hitTest) {
		case HTLEFT:
		case HTRIGHT:
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			return TRUE;
		case HTTOP:
		case HTBOTTOM:
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
			return TRUE;
		case HTTOPLEFT:
		case HTBOTTOMRIGHT:
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
			return TRUE;
		case HTTOPRIGHT:
		case HTBOTTOMLEFT:
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
			return TRUE;
		default:
			break;
		}
		break;
	}
	case WM_GETMINMAXINFO: {
		auto mmi = reinterpret_cast<MINMAXINFO*>(lParam);

		HMONITOR hMon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(hMon, &mi);

		RECT rcWork = mi.rcWork;

		mmi->ptMaxPosition.x = rcWork.left;
		mmi->ptMaxPosition.y = rcWork.top;
		mmi->ptMaxSize.x = rcWork.right - rcWork.left;
		mmi->ptMaxSize.y = rcWork.bottom - rcWork.top;

		return 0;
	}
	case WM_SETTINGCHANGE: {
		if (wParam == SPI_SETWORKAREA) {
			if (IsZoomed(hwnd)) {

				HMONITOR hMon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = { sizeof(mi) };
				GetMonitorInfo(hMon, &mi);
				RECT rcWork = mi.rcWork;

				SetWindowPos(hwnd, NULL, rcWork.left, rcWork.top,
					rcWork.right - rcWork.left, rcWork.bottom - rcWork.top,
					SWP_NOZORDER | SWP_NOACTIVATE);
			}
		}
		break;
	}

	}

	return CallWindowProc(orig_wndproc, hwnd, msg, wParam, lParam);
}


void Application::InitOpenglWindow() {
	glfwInit();
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	windowManager = new WindowManager("IDE NASM", 1280, 720, 100, 100);

}
void Application::Init_IconApplication() {
	// Trying set icon window
	if (isFileExist(L"resources\\icons\\LOGO_small.png")) {
		GLFWimage images[1];
		int width = 0, height = 0;
		images[0].pixels = stbi_load("resources\\icons\\LOGO_small.png", &images[0].width, &images[0].height, 0, 4);
		glfwSetWindowIcon(windowManager->GetMainWindow()->GetHandle(), 1, images);

#ifdef _DEBUG
		std::cout << "Loading ICON:\n\twidth: " << images[0].width << "\n\theight: " << images[0].height << std::endl;
#endif // _DEBUG

		stbi_image_free(images[0].pixels);
	}
	else {
#ifdef _DEBUG
		std::cout << "NOT FIND ICON FILE!!!!\n";
#endif // _DEBUG
	}
}
void Application::Init_BorderWindow() {
	GLFWwindow* win = windowManager->GetMainWindow()->GetHandle();

	HWND hwnd = glfwGetWin32Window(win);
	g_hwnd_to_window[hwnd] = win;

	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style |= WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
	//style |= WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;


	SetWindowLong(hwnd, GWL_STYLE, style);


	MARGINS m = { -1 };
	DwmExtendFrameIntoClientArea(hwnd, &m);


	DWM_WINDOW_CORNER_PREFERENCE pref = DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND;
	DwmSetWindowAttribute(
		hwnd,
		DWMWA_WINDOW_CORNER_PREFERENCE,
		&pref,
		sizeof(pref)
	);


	orig_wndproc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)custom_wndproc);

	glfwSetWindowUserPointer(win, this);


}
void Application::Init_ImGui() {
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();


	ImGui_ImplGlfw_InitForOpenGL(windowManager->GetMainWindow()->GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


	auto& s = ImGui::GetStyle();
	//s.ImageBorderSize = 1.f;
	s.SeparatorTextAlign.x = 0.5f;
	s.SeparatorTextAlign.y = 0.5f;
	s.SeparatorTextBorderSize = 9;

	s.FrameRounding = 6.f;

	s.WindowBorderSize = 1.f;
	s.FrameBorderSize = 1.f;
	s.PopupBorderSize = 1.f;

	s.WindowRounding = 12.f;
	s.ChildRounding = 12.f;

	ImGui::GetIO().IniFilename = NULL;
	ImGui::GetIO().LogFilename = NULL;
}




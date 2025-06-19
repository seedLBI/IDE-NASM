#include "Application.h"



Application::Application() {

	if (HaveFolder(GetPathDirectory() + L"\\repos") == false) {
		CreateFolder(GetPathDirectory() + L"\\repos");
	}

	InitOpenglWindow();

	fontManager = new FontManager;

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
	widget_OutputConsole = new Widget_OutputConsole(buildManager,solution,fontManager, widgetManager_TextEditor);

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
		u8"Сохранить текущий активный файл", 
		KeyCombination({ 
			GLFW_KEY_LEFT_CONTROL, 
			GLFW_KEY_S }, 
		std::bind(&Solution::SaveCurrentFocusedFile, solution))
	);

	keyCombinationHandler->AddCombination(
		u8"Сохранить все файлы", 
		KeyCombination({ 
			GLFW_KEY_LEFT_CONTROL, 
			GLFW_KEY_LEFT_SHIFT,
			GLFW_KEY_S },
		std::bind(&Solution::SaveAllFiles, solution))
	);

	keyCombinationHandler->AddCombination(
		u8"Открыть новый проект",
		KeyCombination({ 
			GLFW_KEY_LEFT_CONTROL, 
			GLFW_KEY_O },
			std::bind(&Solution::Open, solution))
	);

	keyCombinationHandler->AddCombination(
		u8"Компилировать",
		KeyCombination({
			GLFW_KEY_LEFT_CONTROL,
			GLFW_KEY_F7 },
			std::bind(&BuildManager::Compile, buildManager))
	);

	keyCombinationHandler->AddCombination(
		u8"Собрать",
		KeyCombination({
			GLFW_KEY_F7 },
			std::bind(&BuildManager::CompileAndLink, buildManager))
	);

	keyCombinationHandler->AddCombination(
		u8"Запустить без отладки",
		KeyCombination({
			GLFW_KEY_LEFT_CONTROL,
			GLFW_KEY_F5 },
			std::bind(&BuildManager::Run, buildManager))
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

	/*

	GLFWwindow* win = windowManager->GetMainWindow()->GetHandle();
	double mouseX, mouseY;
	glfwGetCursorPos(win, &mouseX, &mouseY);
	int winWidth, winHeight;
	glfwGetWindowSize(win, &winWidth, &winHeight);

	const int border = 5; // Ширина границы для захвата
	bool overLeft = mouseX >= 0 && mouseX < border;
	bool overRight = mouseX >= winWidth - border && mouseX < winWidth;
	bool overTop = mouseY >= 0 && mouseY < border;
	bool overBottom = mouseY >= winHeight - border && mouseY < winHeight;

	// Установка курсора
	if ((overLeft && overTop) || (overRight && overBottom)) {
		glfwSetCursor(win, glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR));
	}
	else if ((overLeft && overBottom) || (overRight && overTop)) {
		glfwSetCursor(win, glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR));
	}
	else if (overLeft || overRight) {
		glfwSetCursor(win, glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR));
	}
	else if (overTop || overBottom) {
		glfwSetCursor(win, glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR));
	}
	else {
		glfwSetCursor(win, nullptr);
	}

	static bool isResizing = false;
	static int resizeDir = 0; // 1: left, 2: right, 3: top, 4: bottom, 5+: углы
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (!isResizing) {
			if (overLeft && overTop) resizeDir = 5;
			else if (overRight && overBottom) resizeDir = 6;
			else if (overLeft && overBottom) resizeDir = 7;
			else if (overRight && overTop) resizeDir = 8;
			else if (overLeft) resizeDir = 1;
			else if (overRight) resizeDir = 2;
			else if (overTop) resizeDir = 3;
			else if (overBottom) resizeDir = 4;
			if (resizeDir > 0) isResizing = true;
		}
	}
	else {
		isResizing = false;
		resizeDir = 0;
	}

	if (isResizing) {
		double newMouseX, newMouseY;
		glfwGetCursorPos(win, &newMouseX, &newMouseY);
		int newX, newY, newWidth, newHeight;
		glfwGetWindowPos(win, &newX, &newY);
		glfwGetWindowSize(win, &newWidth, &newHeight);

		switch (resizeDir) {
		case 1: // Левый край
			newWidth += static_cast<int>(mouseX - newMouseX);
			newX += static_cast<int>(newMouseX - mouseX);
			break;
		case 2: // Правый край
			newWidth += static_cast<int>(newMouseX - mouseX);
			break;
		case 3: // Верхний край
			newHeight += static_cast<int>(mouseY - newMouseY);
			newY += static_cast<int>(newMouseY - mouseY);
			break;
		case 4: // Нижний край
			newHeight += static_cast<int>(newMouseY - mouseY);
			break;
		case 5: // Левый верхний угол
			newWidth += static_cast<int>(mouseX - newMouseX);
			newHeight += static_cast<int>(mouseY - newMouseY);
			newX += static_cast<int>(newMouseX - mouseX);
			newY += static_cast<int>(newMouseY - mouseY);
			break;
		case 6: // Правый нижний угол
			newWidth += static_cast<int>(newMouseX - mouseX);
			newHeight += static_cast<int>(newMouseY - mouseY);
			break;
		case 7: // Левый нижний угол
			newWidth += static_cast<int>(mouseX - newMouseX);
			newHeight += static_cast<int>(newMouseY - mouseY);
			newX += static_cast<int>(newMouseX - mouseX);
			break;
		case 8: // Правый верхний угол
			newWidth += static_cast<int>(newMouseX - mouseX);
			newHeight += static_cast<int>(mouseY - newMouseY);
			newY += static_cast<int>(newMouseY - mouseY);
			break;
		}

		// Ограничение минимального размера окна
		if (newWidth < 300) newWidth = 300;
		if (newHeight < 200) newHeight = 200;
		glfwSetWindowPos(win, newX, newY);
		glfwSetWindowSize(win, newWidth, newHeight);
		mouseX = newMouseX;
		mouseY = newMouseY;
	}
	*/
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


	switch (msg) {
	case WM_NCCALCSIZE: {
		if (wParam) {
			auto params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
			RECT& rc = params->rgrc[0];

			HMONITOR hMon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi = { sizeof(mi) };
			GetMonitorInfo(hMon, &mi);
			RECT wa = mi.rcWork;

			int winW = rc.right - rc.left;
			int winH = rc.bottom - rc.top;
			int workW = wa.right - wa.left;
			int workH = wa.bottom - wa.top;
			const int border = 8;

			bool isSnapped =
				winH == workH &&
				(winW * 2 == workW) &&
				(rc.left == wa.left || rc.right == wa.right);

			if (!isSnapped) {
				rc.left += border;
				rc.top += border;
				rc.right -= border;
				rc.bottom -= border;
			}
			return 0;
		}
		break;
	}
	case WM_NCHITTEST: {
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		ScreenToClient(hwnd, &pt);

		RECT rc; GetClientRect(hwnd, &rc);
		const int border = 8;

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


		RECT wr;
		GetWindowRect(hwnd, &wr);
		int winW = wr.right - wr.left;
		const int buttonCount = 3;
		const int buttonWidth = 40;
		const int totalBtnW = buttonCount * buttonWidth;
		int btnZoneX = winW - totalBtnW;

		bool ColisionWithButton = false;

		auto it = g_hwnd_to_window.find(hwnd);
		if (it != g_hwnd_to_window.end()) {
			GLFWwindow* win = it->second;
			// 3) а из неё достаём Application*
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(win));

			if (app && app->GetPTR_MainMenuBar()->IsPointOverTitleButton(pt)) {
				ColisionWithButton = true;
			}
		}



		if (pt.y >= 0 && pt.y < 60 &&
			((pt.x >= btnZoneX && pt.x < btnZoneX + totalBtnW) || (ColisionWithButton))) {
			return HTCLIENT;
		}





		if (pt.y >= 0 && pt.y < 36) {
			return HTCAPTION;
		}

		break;
	}
	case WM_GETMINMAXINFO: {
		auto mmi = reinterpret_cast<MINMAXINFO*>(lParam);

		HMONITOR hMon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(hMon, &mi);

		RECT rcWork = mi.rcWork;

		DWORD style = GetWindowLong(hwnd, GWL_STYLE);
		DWORD exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		RECT  rcBorder = { 0, 0, 0, 0 };
		AdjustWindowRectEx(&rcBorder, style, FALSE, exStyle);

		int frameX = abs(rcBorder.left) + abs(rcBorder.right);
		int frameY = abs(rcBorder.top) + abs(rcBorder.bottom);

		mmi->ptMaxPosition.x = rcWork.left - abs(rcBorder.left);
		mmi->ptMaxPosition.y = rcWork.top - abs(rcBorder.top);
		mmi->ptMaxSize.x = (rcWork.right - rcWork.left) + frameX;
		mmi->ptMaxSize.y = (rcWork.bottom - rcWork.top) + frameY;

		return HTCLIENT;
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
	GLFWwindow* win = windowManager->GetMainWindow()->GetHandle();
	
	HWND hwnd = glfwGetWin32Window(win);

	g_hwnd_to_window[hwnd] = win;

	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style |= WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
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


	// Trying set icon window
	if (isFileExist(L"resources\\icons\\LOGO_small.png"))	{
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




	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();


	ImGui_ImplGlfw_InitForOpenGL(windowManager->GetMainWindow()->GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


	auto& s = ImGui::GetStyle();
	s.ImageBorderSize = 1.f;

	ImGui::GetIO().IniFilename = NULL;
	ImGui::GetIO().LogFilename = NULL;
}





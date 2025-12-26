#ifndef APPLICATION_H
#define APPLICATION_H

#pragma comment(lib, "winmm.lib")

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_impl_glfw.h"
#include "ThirdParty/ImGui/imgui_impl_opengl3.h"

#include <Windows.h>
#include <glad/glad.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>  // <-- здесь объ€вл€етс€ glfwGetWin32Window
#include <stb_image.h>

#include "Utils/OpenGL/Window/Manager/WindowManager.h"
#include "Utils/Timer/Utils.Timer.Framerate.h"



#include "IDE/GUI/MainMenuBar/MainMenuBar.h"
#include "IDE/GUI/StartupWindow/StartupWindow.h"
#include "IDE/GUI/OutlineStatus/OutlineStatus.h"

#include "IDE/GUI/Widgets/OutputConsole/Widget_OutputConsole.h"
#include "IDE/GUI/Widgets/FilesViewer/Widget_FilesViewer.h"
#include "IDE/GUI/Widgets/TextEditor/WidgetManager_TextEditior.h"
#include "IDE/GUI/Widgets/TextEditor/Widget_TextEditor.h"
#include "IDE/GUI/Widgets/TextEditor/HighlighterSyntax.h"
#include "IDE/GUI/Elements/Notification/NotificationManager.h"


#include "IDE/Core/Setting/Setting.h"
#include "IDE/Core/Solution/Solution.h"
#include "IDE/Core/SaveSystem/SaveSystemManager.h"
#include "IDE/Core/Widget/WidgetManager.h"
#include "IDE/Core/FontManager/FontManager.h"
#include "IDE/Core/LastSolutionManager/LastSolutionManager.h"
#include "IDE/Core/KeyCombination/KeyCombinationHandler.h"
#include "IDE/Core/Theme/ThemeManager/ThemeManager.h"
#include "IDE/Core/Theme/BaseColors/BaseColors.h"
#include "IDE/Core/BuildManager/BuildManager.h"
#include "IDE/Core/PositionWidgetsManager/PositionWidgetsManager.h"





#include <dwmapi.h>
#include <windowsx.h>
#pragma comment(lib, "dwmapi.lib")

// ќригинальный WndProc, чтобы потом делегировать всЄ остальное:
static WNDPROC orig_wndproc = nullptr;
static std::unordered_map<HWND, GLFWwindow*> g_hwnd_to_window;





enum BufferBit {
	COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
	DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
	STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT,
};

class Application
{
public:
	Application();
	~Application();


	void Run();
	MainMenuBar* GetPTR_MainMenuBar();

private:
	FPS_Timer* fps_limiter = nullptr;
	WindowManager* windowManager = nullptr;


	MainMenuBar* mainMenuBar = nullptr;
	StartupWindow* startupWindow = nullptr;
	OutlineStatus* outlineStatus = nullptr;

	FontManager* fontManager = nullptr;

	SaveSystemManager* saveSystemManager = nullptr;
	Setting* setting = nullptr;
	Solution* solution = nullptr;
	LastSolutionManager* lastSolutionManager = nullptr;

	PositionWidgetsManager* positionWidgetsManager = nullptr;

	KeyCombinationHandler* keyCombinationHandler = nullptr;
	NotificationManager* notificationManager = nullptr;
	ThemeManager* themeManager = nullptr;
	BaseColors* baseColors = nullptr;

	BuildManager* buildManager = nullptr;


	WidgetManager* widgetManager = nullptr;
	WidgetManager_TextEditor* widgetManager_TextEditor = nullptr;
	Widget_FilesViewer* widget_FilesViewer = nullptr;
	Widget_OutputConsole* widget_OutputConsole = nullptr;




	const std::wstring Path_SaveData = L"setting.save";



	void InitSaveSystemManager();
	void InitSetting();
	void InitWidgetManager();
	void InitKeyCombinationHandler();
	void InitThemeManager();

	void InitOpenglWindow();
	void Init_IconApplication();
	void Init_BorderWindow();
	void Init_ImGui();
	
	void BeginDraw();
	void EndDraw();



	void Draw();
	void Update();


};




#endif
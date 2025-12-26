#ifndef IDE_MENU_BAR_H
#define IDE_MENU_BAR_H

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"
#include "ThirdParty/IconFontCppHeaders/IconsFontAwesome6.h"
#include "IDE/Core/LastSolutionManager/LastSolutionManager.h"
#include "IDE/Core/Solution/Solution.h"
#include "IDE/Core/Setting/Setting.h"
#include "IDE/Core/BuildManager/BuildManager.h"
#include "IDE/Core/Widget/WidgetManager.h"

#include "IDE/Core/Theme/interface/IThemeLoadable.h"

#include "Utils/OpenGL/Window/Manager/WindowManager.h"
#include "Utils/OpenGL/Texture/Texture.h"

#include <string>

class MainMenuBar : public IThemeLoadable
{
public:
	GLFWwindow* window;
	bool isDragging = false;
	POINT  dragStartPoint;    // { x, y } на экране при нажатии
	int    windowPosStartX;   // позиция окна при нажатии
	int    windowPosStartY;
	bool wasMaximizedOnDrag = false;

	ImVector<ImRect> titleButtonRects;

public:
	MainMenuBar(WindowManager* windowManager, WidgetManager* widgetManager, LastSolutionManager* lastSolutionManager, Solution* solution, Setting* setting, BuildManager* buildManager);
	~MainMenuBar();

	void LoadColors() override;
	std::vector<NamedColor> GetDefaultLightColors() override;
	std::vector<NamedColor> GetDefaultDarkColors() override;


	void Draw();
	void Update();
	
	int GetHeightMenu();

	bool IsMouseOverMenuBar();

	bool IsPointOverTitleButton(const POINT& pt) const;

	void SetFlag_DrawAll();

private:
	WindowManager* windowManager = nullptr;
	WidgetManager* widgetManager = nullptr;
	LastSolutionManager* lastSolutionManager = nullptr;
	Solution* solution = nullptr;
	Setting* setting = nullptr;
	BuildManager* buildManager = nullptr;


	float heightMenu = 0.f;

	void LoadIcon();

	bool DrawingAllButtons = false;

	Texture icon_logo;

	void DrawIcon();
	void DrawProjectName();
	void DrawTitleButtons();

	ImVec2 menuBarPos;
	ImVec2 menuBarSize;



	ImColor color_ProjectBackground{15,15,15,255};
	ImColor color_Logo{ 94, 157, 251, 255 };
};



#endif
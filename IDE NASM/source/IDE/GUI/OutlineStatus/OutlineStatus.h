#ifndef OUTLINE_STATUS_H
#define OUTLINE_STATUS_H


#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"

#include "IDE/Core/Theme/interface/IThemeLoadable.h"

#include "Utils/OpenGL/Window/Manager/WindowManager.h"
#include "Utils/Timer/Utils.Timer.Framerate.h"
#include "Utils/Math/Math.Lerp.Utils.h"

enum class Type_OutlineStatus {
	OutlineStatus_Nothing,
	OutlineStatus_Building,
	OutlineStatus_Running,
	OutlineStatus_Debugging,
};

class OutlineStatus : public IThemeLoadable {
public:
	OutlineStatus(FPS_Timer* fps_limiter, WindowManager* windowManager);
	~OutlineStatus();

	void Draw();
	void Update();


	void LoadColors() override;
	std::vector<NamedColor> GetDefaultLightColors() override;
	std::vector<NamedColor> GetDefaultDarkColors() override;


	void EnableMode_sinAlpha();
	void DisableMode_sinAlpha();

	void SetStatus(const Type_OutlineStatus& status);

private:
	FPS_Timer* fps_limiter = nullptr;
	WindowManager* windowManager = nullptr;

	ImColor CurrentColor;
	ImColor CurrentColor_Lerped;

	ImColor color_Default;
	ImColor color_Building;
	ImColor color_Running;
	ImColor color_Debugging;

	bool flag_MakeSinAlpha = false;


	ImColor GetColorFromCurrentStatus();

	Type_OutlineStatus CurrentStatus = Type_OutlineStatus::OutlineStatus_Nothing;

};



#endif
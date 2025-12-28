#include "OutlineStatus.h"

void OutlineStatus::SetStatus(const Type_OutlineStatus& status) {
	CurrentStatus = status;

}

ImColor OutlineStatus::GetColorFromCurrentStatus() {
	switch (CurrentStatus) {
	case Type_OutlineStatus::OutlineStatus_Nothing: return color_Default;
	case Type_OutlineStatus::OutlineStatus_Building: return color_Building;
	case Type_OutlineStatus::OutlineStatus_Running:  return color_Running;
	case Type_OutlineStatus::OutlineStatus_Debugging: return color_Debugging;
	default:
		return color_Default;
		break;
	}
}

void OutlineStatus::EnableMode_sinAlpha() {
	flag_MakeSinAlpha = true;
}

void OutlineStatus::DisableMode_sinAlpha() {
	flag_MakeSinAlpha = false;
}


OutlineStatus::OutlineStatus(FPS_Timer* fps_limiter, WindowManager* windowManager) : IThemeLoadable("themeItem.outlineStatus") {
	this->fps_limiter = fps_limiter;
	this->windowManager = windowManager;

	IThemeLoadable::InitListWord({
			u8"color.outlineStatus.idle",
			u8"color.outlineStatus.create",
			u8"color.outlineStatus.run",
			u8"color.outlineStatus.debug",
		}
	);

}

void OutlineStatus::LoadColors() {

	for (int i = 0; i < object_colors.colors.size(); i++) {

		const auto toSearch = object_colors.colors[i].nameColor;
		const auto color = object_colors.colors[i].color;

		if (toSearch == u8"color.outlineStatus.idle")
			color_Default = object_colors.colors[i].color;
		else if (toSearch == u8"color.outlineStatus.create")
			color_Building = object_colors.colors[i].color;
		else if (toSearch == u8"color.outlineStatus.run")
			color_Running = object_colors.colors[i].color;
		else if (toSearch == u8"color.outlineStatus.debug")
			color_Debugging = object_colors.colors[i].color;
	}

	CurrentColor = color_Default;
	CurrentColor_Lerped = CurrentColor;
}

std::vector<NamedColor> OutlineStatus::GetDefaultLightColors() {
	std::vector<NamedColor> colors = {
		{u8"color.outlineStatus.idle",	ImColor(163,163,163,150)},
		{u8"color.outlineStatus.create",ImColor(104, 185, 217, 255)},
		{u8"color.outlineStatus.run",	ImColor(255, 176, 0, 255)},
		{u8"color.outlineStatus.debug",	ImColor(0, 255, 18, 255)},
	};
	return colors;
}

std::vector<NamedColor> OutlineStatus::GetDefaultDarkColors() {
	std::vector<NamedColor> colors = {
		{u8"color.outlineStatus.idle",	ImColor(163,163,163,150)},
		{u8"color.outlineStatus.create",ImColor(104, 185, 217, 255)},
		{u8"color.outlineStatus.run",	ImColor(255, 176, 0, 255)},
		{u8"color.outlineStatus.debug",	ImColor(0, 255, 18, 255)},
	};
	return colors;
}

OutlineStatus::~OutlineStatus() {

}


void OutlineStatus::Update() {
	CurrentColor = GetColorFromCurrentStatus();

	if (flag_MakeSinAlpha) {
		CurrentColor.Value.w = (sinf(6.1415f * (float)glfwGetTime()) * 0.5f + 0.5f);

		Lerp(CurrentColor_Lerped, CurrentColor, 20.f, (float)fps_limiter->GetDeltaTime());

		CurrentColor.Value.w = 1.f;
	}
	else
		Lerp(CurrentColor_Lerped, CurrentColor, 20.f, (float)fps_limiter->GetDeltaTime());
}

void OutlineStatus::Draw() {

	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetForegroundDrawList();

	ImVec2 window_size = io.DisplaySize;

	auto rect_min = ImVec2(0.f, 0.f);
	auto rect_max = window_size;

	draw_list->AddRect(rect_min, rect_max, CurrentColor_Lerped, 0.0f, 0, 4.f);
}


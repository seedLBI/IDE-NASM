#include "BaseColors.h"




BaseColors::BaseColors() : IThemeLoadable("themeItem.baseColor") {

	InitListWord({
		u8"color.base.text",
		u8"color.base.textLink",
		u8"color.base.textDisabled",
		u8"color.base.textSelectedBg",
		u8"color.base.windowBg",
		u8"color.base.childBg",
		u8"color.base.popupBg",
		u8"color.base.border",
		u8"color.base.borderShadow",
		u8"color.base.frameBg",
		u8"color.base.frameBgHovered",
		u8"color.base.frameBgActive",
		u8"color.base.titleBg",
		u8"color.base.titleBgActive",
		u8"color.base.titleBgCollapsed",
		u8"color.base.menuBarBg",
		u8"color.base.checkMark",
		u8"color.base.scrollbarBg",
		u8"color.base.scrollbarGrab",
		u8"color.base.scrollbarGrabHovered",
		u8"color.base.scrollbarGrabActive",
		u8"color.base.sliderGrab",
		u8"color.base.sliderGrabActive",
		u8"color.base.button",
		u8"color.base.buttonHovered",
		u8"color.base.buttonActive",
		u8"color.base.header",
		u8"color.base.headerHovered",
		u8"color.base.headerActive",
		u8"color.base.separator",
		u8"color.base.separatorHovered",
		u8"color.base.separatorActive",
		u8"color.base.tab",
		u8"color.base.tabHovered",
		u8"color.base.tabSelected",
		u8"color.base.tabSelectedOverline",
		u8"color.base.tabDimmed",
		u8"color.base.tabDimmedSelected",
		u8"color.base.tabDimmedSelectedOverline",
		u8"color.base.resizeGrip",
		u8"color.base.resizeGripHovered",
		u8"color.base.resizeGripActive",
		u8"color.base.tableHeaderBg",
		u8"color.base.tableBorderStrong",
		u8"color.base.tableBorderLight",
		u8"color.base.tableRowBg",
		u8"color.base.tableRowBgAlt",
		u8"color.base.dockingEmptyBg",
		u8"color.base.cursorText"

		});

	MapNameAndIndex = {
		{u8"color.base.text",						ImGuiCol_Text},
		{u8"color.base.textLink",					ImGuiCol_TextLink},
		{u8"color.base.textDisabled",				ImGuiCol_TextDisabled},
		{u8"color.base.textSelectedBg",				ImGuiCol_TextSelectedBg},
		{u8"color.base.windowBg",					ImGuiCol_WindowBg},
		{u8"color.base.childBg",					ImGuiCol_ChildBg},
		{u8"color.base.popupBg",					ImGuiCol_PopupBg},
		{u8"color.base.border",						ImGuiCol_Border},
		{u8"color.base.borderShadow",				ImGuiCol_BorderShadow},
		{u8"color.base.frameBg",					ImGuiCol_FrameBg},
		{u8"color.base.frameBgHovered",				ImGuiCol_FrameBgHovered},
		{u8"color.base.frameBgActive",				ImGuiCol_FrameBgActive},
		{u8"color.base.titleBg",					ImGuiCol_TitleBg},
		{u8"color.base.titleBgActive",				ImGuiCol_TitleBgActive},
		{u8"color.base.titleBgCollapsed",			ImGuiCol_TitleBgCollapsed},
		{u8"color.base.menuBarBg",					ImGuiCol_MenuBarBg},
		{u8"color.base.checkMark",					ImGuiCol_CheckMark},
		{u8"color.base.scrollbarBg",				ImGuiCol_ScrollbarBg},
		{u8"color.base.scrollbarGrab",				ImGuiCol_ScrollbarGrab},
		{u8"color.base.scrollbarGrabHovered",		ImGuiCol_ScrollbarGrabHovered},
		{u8"color.base.scrollbarGrabActive",		ImGuiCol_ScrollbarGrabActive},
		{u8"color.base.sliderGrab",					ImGuiCol_SliderGrab},
		{u8"color.base.sliderGrabActive",			ImGuiCol_SliderGrabActive},
		{u8"color.base.button",						ImGuiCol_Button},
		{u8"color.base.buttonHovered",				ImGuiCol_ButtonHovered},
		{u8"color.base.buttonActive",				ImGuiCol_ButtonActive},
		{u8"color.base.header",						ImGuiCol_Header},
		{u8"color.base.headerHovered",				ImGuiCol_HeaderHovered},
		{u8"color.base.headerActive",				ImGuiCol_HeaderActive},
		{u8"color.base.separator",					ImGuiCol_Separator},
		{u8"color.base.separatorHovered",			ImGuiCol_SeparatorHovered},
		{u8"color.base.separatorActive",			ImGuiCol_SeparatorActive},
		{u8"color.base.tab",						ImGuiCol_Tab},
		{u8"color.base.tabHovered",					ImGuiCol_TabHovered},
		{u8"color.base.tabSelected",				ImGuiCol_TabSelected},
		{u8"color.base.tabSelectedOverline",		ImGuiCol_TabSelectedOverline},
		{u8"color.base.tabDimmed",					ImGuiCol_TabDimmed},
		{u8"color.base.tabDimmedSelected",			ImGuiCol_TabDimmedSelected},
		{u8"color.base.tabDimmedSelectedOverline",	ImGuiCol_TabDimmedSelectedOverline},
		{u8"color.base.resizeGrip",					ImGuiCol_ResizeGrip},
		{u8"color.base.resizeGripHovered",			ImGuiCol_ResizeGripHovered},
		{u8"color.base.resizeGripActive",			ImGuiCol_ResizeGripActive},
		{u8"color.base.tableHeaderBg",				ImGuiCol_TableHeaderBg},
		{u8"color.base.tableBorderStrong",			ImGuiCol_TableBorderStrong},
		{u8"color.base.tableBorderLight",			ImGuiCol_TableBorderLight},
		{u8"color.base.tableRowBg",					ImGuiCol_TableRowBg},
		{u8"color.base.tableRowBgAlt",				ImGuiCol_TableRowBgAlt},
		{u8"color.base.dockingEmptyBg",				ImGuiCol_DockingEmptyBg},
		{u8"color.base.cursorText",					ImGuiCol_InputTextCursor},
	};

}

BaseColors::~BaseColors() {

}


std::vector<NamedColor> BaseColors::GetDefaultLightColors() {
	std::vector<NamedColor> colors = {
		NamedColor{u8"color.base.text",							ImColor(52,24,0,255)},
		NamedColor{u8"color.base.textLink",						ImColor(82,184,221,255)},
		NamedColor{u8"color.base.textDisabled",					ImColor(152,135,113,255)},
		NamedColor{u8"color.base.textSelectedBg",				ImColor(250,176,66,89)},
		NamedColor{u8"color.base.windowBg",						ImColor(210,198,174,255)},
		NamedColor{u8"color.base.childBg",						ImColor(0,0,0,0)},
		NamedColor{u8"color.base.popupBg",						ImColor(235,220,199,255)},
		NamedColor{u8"color.base.border",						ImColor(79,56,0,255)},
		NamedColor{u8"color.base.borderShadow",					ImColor(0,0,0,0)},
		NamedColor{u8"color.base.frameBg",						ImColor(247,237,212,255)},
		NamedColor{u8"color.base.frameBgHovered",				ImColor(250,176,66,102)},
		NamedColor{u8"color.base.frameBgActive",				ImColor(234,175,61,171)},
		NamedColor{u8"color.base.titleBg",						ImColor(234,221,190,255)},
		NamedColor{u8"color.base.titleBgActive",				ImColor(177,165,138,255)},
		NamedColor{u8"color.base.titleBgCollapsed",				ImColor(211,200,176,130)},
		NamedColor{u8"color.base.menuBarBg",					ImColor(215,205,182,255)},
		NamedColor{u8"color.base.checkMark",					ImColor(116,100,67,255)},
		NamedColor{u8"color.base.scrollbarBg",					ImColor(255,239,218,135)},
		NamedColor{u8"color.base.scrollbarGrab",				ImColor(182,168,151,204)},
		NamedColor{u8"color.base.scrollbarGrabHovered",			ImColor(125,116,99,204)},
		NamedColor{u8"color.base.scrollbarGrabActive",			ImColor(125,109,83,255)},
		NamedColor{u8"color.base.sliderGrab",					ImColor(250,191,66,199)},
		NamedColor{u8"color.base.sliderGrabActive",				ImColor(204,176,117,153)},
		NamedColor{u8"color.base.button",						ImColor(250,191,66,102)},
		NamedColor{u8"color.base.buttonHovered",				ImColor(250,183,66,255)},
		NamedColor{u8"color.base.buttonActive",					ImColor(250,165,15,255)},
		NamedColor{u8"color.base.header",						ImColor(250,172,66,79)},
		NamedColor{u8"color.base.headerHovered",				ImColor(250,179,66,204)},
		NamedColor{u8"color.base.headerActive",					ImColor(250,183,66,255)},
		NamedColor{u8"color.base.separator",					ImColor(97,84,67,158)},
		NamedColor{u8"color.base.separatorHovered",				ImColor(204,133,36,199)},
		NamedColor{u8"color.base.separatorActive",				ImColor(204,133,36,255)},
		NamedColor{u8"color.base.tab",							ImColor(216,205,189,237)},
		NamedColor{u8"color.base.tabHovered",					ImColor(250,176,66,204)},
		NamedColor{u8"color.base.tabSelected",					ImColor(224,198,154,255)},
		NamedColor{u8"color.base.tabSelectedOverline",			ImColor(250,187,66,255)},
		NamedColor{u8"color.base.tabDimmed",					ImColor(237,227,206,252)},
		NamedColor{u8"color.base.tabDimmedSelected",			ImColor(232,216,189,255)},
		NamedColor{u8"color.base.tabDimmedSelectedOverline",	ImColor(255,183,66,255)},
		NamedColor{u8"color.base.resizeGrip",					ImColor(87,80,68,43)},
		NamedColor{u8"color.base.resizeGripHovered",			ImColor(250,179,66,171)},
		NamedColor{u8"color.base.resizeGripActive",				ImColor(250,179,66,242)},
		NamedColor{u8"color.base.tableHeaderBg",				ImColor(250,230,199,255)},
		NamedColor{u8"color.base.tableBorderStrong",			ImColor(163,156,145,255)},
		NamedColor{u8"color.base.tableBorderLight",				ImColor(189,184,173,255)},
		NamedColor{u8"color.base.tableRowBg",					ImColor(0,0,0,0)},
		NamedColor{u8"color.base.tableRowBgAlt",				ImColor(97,54,0,23)},
		NamedColor{u8"color.base.dockingEmptyBg",				ImColor(49,31,8,255)},
		NamedColor{u8"color.base.cursorText",					ImColor(0.f,   0.f,   0.f,   1.f)}
	};
	return colors;

}
std::vector<NamedColor> BaseColors::GetDefaultDarkColors() {

	std::vector<NamedColor> colors = {
		NamedColor{u8"color.base.text",							ImColor(1.00f, 1.00f, 1.00f, 1.00f)},
		NamedColor{u8"color.base.textLink",						ImColor(0.26f, 0.59f, 0.98f, 1.00f)},
		NamedColor{u8"color.base.textDisabled",					ImColor(0.50f, 0.50f, 0.50f, 1.00f)},
		NamedColor{u8"color.base.textSelectedBg",				ImColor(0.26f, 0.59f, 0.98f, 0.35f)},
		NamedColor{u8"color.base.windowBg",						ImColor(0.06f, 0.06f, 0.06f, 0.94f)},
		NamedColor{u8"color.base.childBg",						ImColor(0.00f, 0.00f, 0.00f, 0.00f)},
		NamedColor{u8"color.base.popupBg",						ImColor(0.08f, 0.08f, 0.08f, 0.94f)},
		NamedColor{u8"color.base.border",						ImColor(0.43f, 0.43f, 0.50f, 0.50f)},
		NamedColor{u8"color.base.borderShadow",					ImColor(0.00f, 0.00f, 0.00f, 0.00f)},
		NamedColor{u8"color.base.frameBg",						ImColor(0.16f, 0.29f, 0.48f, 0.54f)},
		NamedColor{u8"color.base.frameBgHovered",				ImColor(0.26f, 0.59f, 0.98f, 0.40f)},
		NamedColor{u8"color.base.frameBgActive",				ImColor(0.26f, 0.59f, 0.98f, 0.67f)},
		NamedColor{u8"color.base.titleBg",						ImColor(0.04f, 0.04f, 0.04f, 1.00f)},
		NamedColor{u8"color.base.titleBgActive",				ImColor(0.16f, 0.29f, 0.48f, 1.00f)},
		NamedColor{u8"color.base.titleBgCollapsed",				ImColor(0.00f, 0.00f, 0.00f, 0.51f)},
		NamedColor{u8"color.base.menuBarBg",					ImColor(0.14f, 0.14f, 0.14f, 1.00f)},
		NamedColor{u8"color.base.checkMark",					ImColor(0.26f, 0.59f, 0.98f, 1.00f)},
		NamedColor{u8"color.base.scrollbarBg",					ImColor(0.02f, 0.02f, 0.02f, 0.53f)},
		NamedColor{u8"color.base.scrollbarGrab",				ImColor(0.31f, 0.31f, 0.31f, 1.00f)},
		NamedColor{u8"color.base.scrollbarGrabHovered",			ImColor(0.41f, 0.41f, 0.41f, 1.00f)},
		NamedColor{u8"color.base.scrollbarGrabActive",			ImColor(0.51f, 0.51f, 0.51f, 1.00f)},
		NamedColor{u8"color.base.sliderGrab",					ImColor(0.24f, 0.52f, 0.88f, 1.00f)},
		NamedColor{u8"color.base.sliderGrabActive",				ImColor(0.26f, 0.59f, 0.98f, 1.00f)},
		NamedColor{u8"color.base.button",						ImColor(0.26f, 0.59f, 0.98f, 0.40f)},
		NamedColor{u8"color.base.buttonHovered",				ImColor(0.26f, 0.59f, 0.98f, 1.00f)},
		NamedColor{u8"color.base.buttonActive",					ImColor(0.06f, 0.53f, 0.98f, 1.00f)},
		NamedColor{u8"color.base.header",						ImColor(0.26f, 0.59f, 0.98f, 0.31f)},
		NamedColor{u8"color.base.headerHovered",				ImColor(0.26f, 0.59f, 0.98f, 0.80f)},
		NamedColor{u8"color.base.headerActive",					ImColor(0.26f, 0.59f, 0.98f, 1.00f)},
		NamedColor{u8"color.base.separator",					ImColor(0.43f, 0.43f, 0.50f, 0.50f)},
		NamedColor{u8"color.base.separatorHovered",				ImColor(0.10f, 0.40f, 0.75f, 0.78f)},
		NamedColor{u8"color.base.separatorActive",				ImColor(0.10f, 0.40f, 0.75f, 1.00f)},
		NamedColor{u8"color.base.tab",							ImColor(0.18f, 0.35f, 0.58f, 0.86f)},
		NamedColor{u8"color.base.tabHovered",					ImColor(0.26f, 0.59f, 0.98f, 0.80f)},
		NamedColor{u8"color.base.tabSelected",					ImColor(0.20f, 0.41f, 0.68f, 1.00f)},
		NamedColor{u8"color.base.tabSelectedOverline",			ImColor(0.26f, 0.59f, 0.98f, 1.00f)},
		NamedColor{u8"color.base.tabDimmed",					ImColor(0.07f, 0.10f, 0.15f, 0.97f)},
		NamedColor{u8"color.base.tabDimmedSelected",			ImColor(0.14f, 0.26f, 0.42f, 1.00f)},
		NamedColor{u8"color.base.tabDimmedSelectedOverline",	ImColor(0.50f, 0.50f, 0.50f, 1.00f)},
		NamedColor{u8"color.base.resizeGrip",					ImColor(0.26f, 0.59f, 0.98f, 0.20f)},
		NamedColor{u8"color.base.resizeGripHovered",			ImColor(0.26f, 0.59f, 0.98f, 0.67f)},
		NamedColor{u8"color.base.resizeGripActive",				ImColor(0.26f, 0.59f, 0.98f, 0.95f)},
		NamedColor{u8"color.base.tableHeaderBg",				ImColor(0.19f, 0.19f, 0.20f, 1.00f)},
		NamedColor{u8"color.base.tableBorderStrong",			ImColor(0.31f, 0.31f, 0.35f, 1.00f)},
		NamedColor{u8"color.base.tableBorderLight",				ImColor(0.23f, 0.23f, 0.25f, 1.00f)},
		NamedColor{u8"color.base.tableRowBg",					ImColor(0.00f, 0.00f, 0.00f, 0.00f)},
		NamedColor{u8"color.base.tableRowBgAlt",				ImColor(1.00f, 1.00f, 1.00f, 0.06f)},
		NamedColor{u8"color.base.dockingEmptyBg",				ImColor(0.20f, 0.20f, 0.20f, 1.00f)},
		NamedColor{u8"color.base.cursorText",					ImColor(1.f,   1.f,   1.f,   1.f  )}
	};

	return colors;

}
void BaseColors::LoadColors() {

	ImVec4* colorss = ImGui::GetStyle().Colors;

	std::string toSearch;

	for (int i = 0; i < object_colors.colors.size(); i++) {

		toSearch = object_colors.colors[i].nameColor;

		if (MapNameAndIndex.contains(toSearch)) {
			colorss[MapNameAndIndex[toSearch]] = object_colors.colors[i].color.Value;
		}
	}
}
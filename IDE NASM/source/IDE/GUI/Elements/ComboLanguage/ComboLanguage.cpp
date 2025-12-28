#include "ComboLanguage.h"

#include "IDE/Core/LocalisationManager/LocalisationManager.h"

ComboLanguage::ComboLanguage(GLFWwindow* window) {
	this->window = window;
	Init();

}

ComboLanguage::~ComboLanguage() {

}

void ComboLanguage::SetCurrentLanguage(const std::string& lang) {
	CurrentLanguage = lang;
}

ComboLanguage::flag_texture_data ComboLanguage::GetFlagData(const std::string& flag_abriv) {

	flag_texture_data result;


	const ImVec2 icon_size(44, 30);
	const ImVec2 atlas_size_inv(1.0f / texture_flags.GetWidth(), 1.0f / texture_flags.GetHeight());




	const float f_x = (float)data_text[flag_abriv]["frame"]["x"].get<int>();
	const float f_y = (float)data_text[flag_abriv]["frame"]["y"].get<int>();
	const float f_w = (float)data_text[flag_abriv]["frame"]["w"].get<int>();
	const float f_h = (float)data_text[flag_abriv]["frame"]["h"].get<int>();



	ImVec2 uv0(
		f_x * atlas_size_inv.x,
		1.f - f_y * atlas_size_inv.y);
	ImVec2 uv1(
		(f_x + f_w) * atlas_size_inv.x,
		1.f - (f_y + f_h) * atlas_size_inv.y);

	result.icon_size = icon_size;
	result.uv0 = uv0;
	result.uv1 = uv1;


	return result;
}

bool ComboLanguage::FlagSelectableFromAtlas(const char* label, const flag_texture_data& flag, bool selected, ImVec2* out_size)
{
	const ImTextureID atlas_tex = texture_flags.GetID();
	ImVec2 txt_size = ImGui::CalcTextSize(label);

	const float desired_height = ImGui::GetTextLineHeight() + 1.5f;
	const float aspect_ratio = flag.icon_size.x / flag.icon_size.y;
	const ImVec2 icon_size(desired_height * aspect_ratio, desired_height);

	const ImGuiStyle& style = ImGui::GetStyle();
	const float pad = style.ItemInnerSpacing.x;
	const float full_width = ImGui::GetContentRegionAvail().x;

	const float item_height = icon_size.y + style.FramePadding.y * 2.0f;
	ImVec2 total_size(full_width, item_height);

	if (out_size) *out_size = total_size;

	ImGui::PushID(label);
	bool clicked = ImGui::Selectable("##flagsel", selected,
		ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap,
		total_size);

	const ImVec2 min = ImGui::GetItemRectMin();
	const ImVec2 max = ImGui::GetItemRectMax();
	ImGui::PopID();

	ImDrawList* dl = ImGui::GetWindowDrawList();
	const float flag_y_offset = (total_size.y - icon_size.y) * 0.5f + 2.f;
	dl->AddImage(atlas_tex,
		ImVec2(min.x + style.FramePadding.x, min.y + flag_y_offset),
		ImVec2(min.x + icon_size.x + style.FramePadding.x, min.y + flag_y_offset + icon_size.y),
		flag.uv0, flag.uv1);

	const float text_x = min.x + icon_size.x + pad * 2.0f + style.FramePadding.x;
	const float text_y_offset = (total_size.y - txt_size.y) * 0.5f;
	dl->AddText(
		ImVec2(text_x, min.y + text_y_offset),
		ImGui::GetColorU32(ImGuiCol_Text),
		label);

	return clicked;
}

std::string ComboLanguage::GetCurrentLanguage() {
	return CurrentLanguage;
}

void ComboLanguage::Draw(const std::string& label) {
	if (ImGui::BeginCombo(
		std::string(label + "##ComboBox").c_str(), 
		data_text[CurrentLanguage]["description"].get<std::string>().c_str(), 
		ImGuiComboFlags_HeightSmall)) 
	{


		for (auto& [key, value] : data_text.items()) {


			bool was_selected = (key == CurrentLanguage);
			if (FlagSelectableFromAtlas(data_text[key]["description"].get<std::string>().c_str(), GetFlagData(key), was_selected))
			{
				CurrentLanguage = key;
			}

			if (was_selected)
				ImGui::SetItemDefaultFocus();


		}



		ImGui::EndCombo();
	}
}


void ComboLanguage::Init() {

	if (isFileExist("resources\\icons\\flags\\image.png")) {

		TextureSetting settingFlags;
		settingFlags.Min = TextureFilter::NEAREST;
		settingFlags.Max = TextureFilter::NEAREST;
		settingFlags.WrapX = TextureWrap::CLAMP_TO_EDGE;
		settingFlags.WrapY = TextureWrap::CLAMP_TO_EDGE;
		texture_flags.SetSetting(settingFlags);
		texture_flags.LoadTexture("resources\\icons\\flags\\image.png");
		texture_flags.Init();
	}
	else {
		MessageBoxW(
			glfwGetWin32Window(window),
			(stringUTF8_to_wstring(tr("error.comboLanguages.loadFlags")) + L" (resources\\icons\\flags\\image.png)").c_str(),
			 stringUTF8_to_wstring(tr("error.comboLanguages.loadFlags.title")).c_str(),
			MB_ICONERROR | MB_OK);
		std::abort();
	}


	if (isFileExist("resources\\json\\LanguageChoose.json")) {
		std::ifstream ifn;
		ifn.open("resources\\json\\LanguageChoose.json");
		data_text = nlohmann::json::parse(ifn);
		ifn.close();
	}
	else {
		MessageBoxW(
			glfwGetWin32Window(window),
			(stringUTF8_to_wstring(tr("error.comboLanguages.loadData")) + L"А где данные о языках? (resources\\json\\LanguageChoose.json)").c_str(),
			stringUTF8_to_wstring(tr("error.comboLanguages.loadData.title")).c_str(),
			MB_ICONERROR | MB_OK);
		std::abort();
	}

	
}

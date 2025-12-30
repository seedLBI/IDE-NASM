#include "StartupWindow.h"
#include "IDE/Core/Solution/Solution.h"

#include "IDE/Core/LocalisationManager/LocalisationManager.h"

StartupWindow::StartupWindow(FontManager* fontManager, Solution* solution) {
	this->fontManager = fontManager;
	this->solution = solution;

	Color_Favorite = ImVec4(175.f / 255.f, 110.f / 255.f, 225.f / 255.f, 1.f);
	Color_Favorite_Hovered = ImVec4(201.f / 255.f, 135.f / 255.f, 255.f / 255.f, 1.f);
	Color_Outline = ImVec4(1.f, 0.f, 0.f, 1.f);


}


StartupWindow::~StartupWindow() {

}


void StartupWindow::SetPTR_last_solutions(std::vector<SolutionInfo>* last_solutions) {
	this->last_solutions = last_solutions;
}



bool StartupWindow::NeedClose() {
	return isNeedToBeClosed;
}

bool StartupWindow::DrawListObject(SolutionInfo& info, const int& index, bool& btn_pressed_fav, bool& btn_pressed_del, bool& btn_pressed_main) {

	const char* label = info.name.c_str();

	


	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *ImGui::GetCurrentContext();
	const ImGuiStyle& style = g.Style;

	ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);
	ImVec2 padding = style.FramePadding;
	float btn_min_height = label_size.y + padding.y * 2;
	float available_width = ImGui::GetContentRegionAvail().x - 4.f;

	ImVec2 main_button_size = ImVec2(available_width, btn_min_height);

	float extra_btn_size = btn_min_height * 0.8f;
	float spacing = style.ItemInnerSpacing.x;
	float extra_total_width = extra_btn_size * 2 + spacing;

	ImVec2 pos = window->DC.CursorPos;
	ImRect bb(pos, ImVec2(pos.x + main_button_size.x, pos.y + main_button_size.y));



	ImGuiID id_main = window->GetID(std::string(info.name + " ##base" + std::to_string(index)).c_str());

	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id_main))
		return false;




	ImRect main_bb(pos, ImVec2(bb.Max.x - extra_total_width, bb.Max.y));
	float extra_x = bb.Max.x - extra_total_width;
	ImRect bb_delete(ImVec2(extra_x, bb.Min.y), ImVec2(extra_x + extra_btn_size, bb.Max.y));
	extra_x += extra_btn_size + spacing;
	ImRect bb_fav(ImVec2(extra_x, bb.Min.y), ImVec2(extra_x + extra_btn_size, bb.Max.y));





	bool hovered_main = false, pressed_main = false;
	if (ImGui::ButtonBehavior(main_bb, id_main, &hovered_main, &pressed_main, NULL)) {
		btn_pressed_main = true;
	}
	else {
		btn_pressed_main = false;
	}

	

	ImU32 col_bg = ImGui::GetColorU32((hovered_main) ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	ImGui::RenderFrame(bb.Min, bb.Max, col_bg, true, style.FrameRounding);


	bool ElementChoosed = false;

	if (index == CurrentIndexHovered)
		ElementChoosed = true;

	if (ElementChoosed) {
		
		window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_TextDisabled), style.FrameRounding, 0, 3.f);
	}
	

	ImVec2 text_pos = ImVec2(bb.Min.x + padding.x, bb.Min.y + padding.y);
	ImGui::RenderText(text_pos, label);





	std::string str_id_del = "##delete" + std::string(info.isFavorite == true ? "##fav" : "##last") + std::to_string(index);

	ImGuiID id_del = window->GetID(str_id_del.c_str());

	if (!ImGui::ItemAdd(bb_delete, id_del))
		return false;

	bool hovered_del = false, pressed_del = false;
	if (ImGui::ButtonBehavior(bb_delete, id_del, &hovered_del, &pressed_del, NULL)) {
		btn_pressed_del = true;
	}
	else {
		btn_pressed_del = false;
	}


	const char* del_label = ICON_FA_TRASH;
	ImVec2 txt_size = ImGui::CalcTextSize(del_label, nullptr, true);
	ImVec2 txt_pos_del = ImVec2(
		bb_delete.Min.x + (extra_btn_size - txt_size.x) * 0.5f, 
		bb_delete.Min.y + (extra_btn_size - txt_size.y) * 0.5f);


	if (hovered_del)
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_Text));
	else
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));

	ImGui::RenderText(txt_pos_del, del_label);

	ImGui::PopStyleColor();




	std::string str_id_fav = "##favorite" + std::string(info.isFavorite == true ? "##fav" : "##last") + std::to_string(index);

	ImGuiID id_fav = window->GetID(str_id_fav.c_str());


	if (!ImGui::ItemAdd(bb_fav, id_fav))
		return false;

	bool hovered_fav = false, pressed_fav = false;
	if (ImGui::ButtonBehavior(bb_fav, id_fav, &hovered_fav, &pressed_fav, NULL)) {
		btn_pressed_fav = true;
	}
	else {
		btn_pressed_fav = false;
	}

	const char* fav_label = ICON_FA_STAR;
	txt_size = ImGui::CalcTextSize(fav_label, nullptr, true);
	ImVec2 txt_pos_fav = ImVec2(bb_fav.Min.x + (extra_btn_size - txt_size.x) * 0.5f,
		bb_fav.Min.y + (extra_btn_size - txt_size.y) * 0.5f);



	if (hovered_fav) {
		ImGui::PushStyleColor(ImGuiCol_Text, Color_Favorite_Hovered);
	}
	else {
		if (info.isFavorite)
			ImGui::PushStyleColor(ImGuiCol_Text, Color_Favorite);
		else
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
	}

	ImGui::RenderText(txt_pos_fav, fav_label);

	ImGui::PopStyleColor();


	if (hovered_fav == false && hovered_del == false && hovered_main == true) {

		CurrentIndexHovered = index;

	}


	return false;
}

void DrawBackgroundPattern() {
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	if (!draw_list) return;

	ImVec2 screen_min = ImVec2(0, 0);
	ImVec2 screen_size = ImGui::GetIO().DisplaySize;
	ImVec2 screen_max = screen_size;

	float angle_deg = 45.0f;
	float angle_rad = angle_deg * IM_PI / 180.0f;
	ImVec2 dir(cosf(angle_rad), sinf(angle_rad));
	float dir_len = sqrtf(dir.x * dir.x + dir.y * dir.y);
	dir.x /= dir_len;
	dir.y /= dir_len;

	ImVec2 perp(-dir.y, dir.x);

	float line_spacing = 120.0f;
	float speed = 50.0f;
	float time = ImGui::GetTime();
	ImU32 color = IM_COL32(128, 128, 128, 128);
	float text_size = 160.0f;

	ImFont* font = ImGui::GetFont();
	const char* text1 = "IDE ";
	const char* text2 = "NASM ";
	ImVec2 size1 = font->CalcTextSizeA(text_size, FLT_MAX, 0.0f, text1);
	ImVec2 size2 = font->CalcTextSizeA(text_size, FLT_MAX, 0.0f, text2);
	float repeat_dist1 = size1.x;
	float repeat_dist2 = size2.x;

	float diag = sqrtf(screen_size.x * screen_size.x + screen_size.y * screen_size.y);

	ImVec2 corners[4] = { screen_min, ImVec2(screen_max.x, screen_min.y), screen_max, ImVec2(screen_min.x, screen_max.y) };
	float min_t = FLT_MAX;
	float max_t = -FLT_MAX;
	for (int c = 0; c < 4; ++c) {
		float t = corners[c].x * perp.x + corners[c].y * perp.y;
		min_t = ImMin(min_t, t);
		max_t = ImMax(max_t, t);
	}

	int num_lines = static_cast<int>((max_t - min_t) / line_spacing) + 4;
	for (int i = 0; i < num_lines; ++i) {
		float t = min_t + (i - 2) * line_spacing;
		ImVec2 base = ImVec2(perp.x * t, perp.y * t);

		bool is_even = (i % 2 == 0);
		const char* text = is_even ? text1 : text2;
		float repeat_dist = is_even ? repeat_dist1 : repeat_dist2;
		float dir_sign = is_even ? 1.0f : -1.0f;
		float offset = fmodf(time * speed * dir_sign, repeat_dist);
		if (offset < 0.0f) offset += repeat_dist;

		float text_angle = angle_rad;

		float sin_a = sinf(text_angle);
		float cos_a = cosf(text_angle);

		float min_s = FLT_MAX;
		float max_s = -FLT_MAX;
		for (int c = 0; c < 4; ++c) {
			ImVec2 diff = ImVec2(corners[c].x - base.x, corners[c].y - base.y);
			float s = diff.x * dir.x + diff.y * dir.y;
			min_s = ImMin(min_s, s);
			max_s = ImMax(max_s, s);
		}
		min_s -= diag;
		max_s += diag;

		int min_k = static_cast<int>(floorf(min_s / repeat_dist));
		int max_k = static_cast<int>(ceilf(max_s / repeat_dist));

		for (int k = min_k - 1; k <= max_k + 1; ++k) {
			float s = k * repeat_dist + offset;
			ImVec2 start_pos =  ImVec2(base.x + s * dir.x, base.y + s * dir.y);

			ImVec2 curr_pos = start_pos;
			for (const char* p = text; *p; ++p) {
				const ImFontGlyph* glyph = font->FindGlyph((ImWchar)*p);
				if (!glyph) continue;

				float scale = text_size / font->FontSize;
				float w = (glyph->X1 - glyph->X0) * scale;
				float h = (glyph->Y1 - glyph->Y0) * scale;

				if (w <= 0.0f || h <= 0.0f) {
					curr_pos.x += (glyph->AdvanceX * scale) * dir.x;
					curr_pos.y += (glyph->AdvanceX * scale) * dir.y;
					continue;
				}

				ImVec2 center =  ImVec2(curr_pos.x + w * 0.5f, curr_pos.y + h * 0.5f);

				ImVec2 rel[4] = {
					ImVec2(-w * 0.5f, -h * 0.5f),
					ImVec2(w * 0.5f, -h * 0.5f),
					ImVec2(w * 0.5f,  h * 0.5f),
					ImVec2(-w * 0.5f,  h * 0.5f)
				};

				ImVec2 verts[4];
				for (int v = 0; v < 4; ++v) {
					float rx = rel[v].x * cos_a - rel[v].y * sin_a;
					float ry = rel[v].x * sin_a + rel[v].y * cos_a;
					verts[v] = ImVec2(center.x + rx, center.y + ry);
				}

				ImVec2 uv0(glyph->U0, glyph->V0);
				ImVec2 uv1(glyph->U1, glyph->V0);
				ImVec2 uv2(glyph->U1, glyph->V1);
				ImVec2 uv3(glyph->U0, glyph->V1);

				draw_list->PrimReserve(6, 4);
				draw_list->PrimQuadUV(verts[0], verts[1], verts[2], verts[3], uv0, uv1, uv2, uv3, color);

				curr_pos.x += (glyph->AdvanceX * scale) * dir.x;
				curr_pos.y += (glyph->AdvanceX * scale) * dir.y;
			}
		}
	}
}


std::string StartupWindow::timestamp_to_DateAndTime_str(const uint64_t& timestamp) {
	time_t ts = static_cast<time_t>(timestamp);
	struct tm dt;
	localtime_s(&dt,&ts);

	int year = dt.tm_year + 1900;
	int month = dt.tm_mon + 1;
	int day = dt.tm_mday;
	int hour = dt.tm_hour;
	int minute = dt.tm_min;
	int second = dt.tm_sec;


	std::string str_day = std::to_string(day);
	PadLeft(str_day, '0', 2);

	std::string str_month = std::to_string(month);
	PadLeft(str_month, '0', 2);

	std::string str_hour = std::to_string(hour);
	PadLeft(str_hour, '0', 2);

	std::string str_minute = std::to_string(minute);
	PadLeft(str_minute, '0', 2);


	return 
		str_day + "." +
		str_month + "." +
		std::to_string(year) +
		" " + 
		str_hour + ":" +
		str_minute;


}

std::string StartupWindow::timestamp_to_Time_str(const uint64_t& timestamp) {


	int hour   = static_cast<int>(timestamp / uint64_t(60*60) );
	int minute = static_cast<int>(timestamp / uint64_t(60)) - hour * 60;
	int second = static_cast<int>(timestamp - minute * 60 - hour*60*60);

	std::string str_hour = std::to_string(hour);
	std::string str_minute = std::to_string(minute);
	std::string str_second = std::to_string(second);


	std::string result;

	if (hour > 0) {
		result += str_hour + u8" час. ";
	}
	if (minute > 0)
	{
		result += str_minute + u8" мин. ";
	}

	if (second != 0)
	{
		result += str_second + u8" сек. ";
	}
	else {
		if (minute == 0 && hour == 0)
			result += str_second + u8" сек. ";
	}


	return result;


}




void StartupWindow::SaveAllSolutions() {
	for (int i = 0; i < last_solutions->size(); i++) {
		SaveSolution(last_solutions->at(i));
	}
}

void StartupWindow::TryingOpen() {
	SaveAllSolutions();
	if (solution->Open()) {
		isNeedToBeClosed = true;
	}
}
void StartupWindow::TryingOpen(const std::wstring& fullpath) {
	SaveAllSolutions();
	if (solution->OpenFromPath(fullpath)) {
		isNeedToBeClosed = true;
	}
}


void StartupWindow::Draw() {

	if (solution->isCreated()){
		isNeedToBeClosed = true;
		return;
	}


	static const ImGuiWindowFlags flagsWindow =
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse;

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	ImVec2 size = ImGui::GetMainViewport()->WorkSize;
	ImGui::SetNextWindowSize(ImVec2(size.x / 1.3f, size.y / 1.3f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 6.0f));


	DrawBackgroundPattern();


	if (ImGui::Begin((tr("startupWindow.title") + u8" ###CHOOSING_PROJECT").c_str(), 0, flagsWindow)) {

		center.x -= ImGui::GetCurrentWindow()->Size.x / 2;
		center.y -= ImGui::GetCurrentWindow()->Size.y / 2;
		ImGui::GetCurrentWindow()->Pos = center;




		ImGui::BeginChild("left_ChooseProject", ImVec2(ImGui::GetCurrentWindow()->Size.x / 1.7f, 0));



		float CurrentFontSize = ImGui::GetFontSize();

		float minValueForCalcFontSize = (std::min)(ImGui::GetCurrentWindow()->Size.x / 9, ImGui::GetCurrentWindow()->Size.y / 15);

		float AddFontSize = minValueForCalcFontSize / CurrentFontSize;



		fontManager->Push_Bold();

			ImGui::SetWindowFontScale(AddFontSize);

			if (ImGui::Button(tr("startupWindow.button.create").c_str(), ImVec2(ImGui::GetCurrentWindow()->Size.x / 2, ImGui::GetCurrentWindow()->Size.y / 8))) {
				solution->Create();
			}
			ImGui::SameLine();
			if (ImGui::Button(tr("startupWindow.button.open").c_str(), ImVec2(ImGui::GetCurrentWindow()->Size.x / 2 - 10.f, ImGui::GetCurrentWindow()->Size.y / 8))) {
				TryingOpen();
			}

			ImGui::SetWindowFontScale(1.f);

		ImGui::PopFont();


			ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 3.f);

			int LastFavorite = 0;

			bool HaveFavorite = false;

			for (int i = 0; i < last_solutions->size(); i++){
				if (last_solutions->at(i).isFavorite){
					HaveFavorite = true;
					break;
				}
			}

			for (int i = 0; i < last_solutions->size(); i++) {

				if (last_solutions->at(i).isFavorite == false) {
					LastFavorite = i;
					break;
				}

				LastFavorite = i + 1;
			}



			if (HaveFavorite) {
				ImGui::PushStyleColor(ImGuiCol_Text, Color_Favorite);


				if (ImGui::TreeNodeEx(std::string(tr("startupWindow.list.favorite") + " " + std::string(ICON_FA_STAR)).c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth)) {
					ImGui::PopStyleColor();


					for (int i = 0; i < last_solutions->size(); i++) {

						if (last_solutions->at(i).isFavorite == false) {
							LastFavorite = i;
							break;
						}

						bool pressed_main = false, pressed_fav = false, pressed_del = false;
						DrawListObject(last_solutions->at(i),i, pressed_fav, pressed_del, pressed_main);


						if (pressed_main) {
							Index_pressed_main = i;
						}
						if (pressed_fav) {
							Index_pressed_fav = i;
						}
						if (pressed_del) {
							Index_pressed_del = i;
						}

					}


					ImGui::TreePop();
				}
				else {
					ImGui::PopStyleColor();
				}
			}



			if (ImGui::TreeNodeEx(tr("startupWindow.list.last").c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth)) {

				for (int i = LastFavorite; i < last_solutions->size(); i++)
				{
					bool pressed_main = false, pressed_fav = false, pressed_del = false;

					DrawListObject(last_solutions->at(i), i, pressed_fav, pressed_del, pressed_main);

					if (pressed_main) {
						Index_pressed_main = i;
					}
					if (pressed_fav) {
						Index_pressed_fav = i;
					}
					if (pressed_del) {
						Index_pressed_del = i;
					}

				}

				

				ImGui::TreePop();
			}


		ImGui::EndChild();



		ImGui::SameLine();

		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 3.f);

		ImGui::SameLine();



		ImGui::BeginChild("right_ChooseProject", ImVec2(0, 0));



		SolutionInfo* show = nullptr;



		if (last_solutions->empty() == false && CurrentIndexHovered != -1) {
			show = &last_solutions->at(CurrentIndexHovered);
		}


		float available_width = ImGui::GetContentRegionAvail().x;
		ImVec2 main_button_size = ImVec2(available_width, ImGui::GetCurrentWindow()->Size.y / 8);


		ImGuiContext& g = *ImGui::GetCurrentContext();
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		const ImGuiStyle& style = g.Style;


		ImVec2 pos = window->DC.CursorPos;
		ImRect bb(pos, ImVec2(pos.x + main_button_size.x, pos.y + main_button_size.y));

		//	ImU32 col_bg = ImGui::GetColorU32((info.hovered_main) ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_Button), true, style.FrameRounding);



		ImGui::SetWindowFontScale(AddFontSize);

		fontManager->Push_Bold();

		std::string text_info = tr("startupWindow.information");

		ImVec2 txt_size = ImGui::CalcTextSize(text_info.c_str(), nullptr, true);

		ImGuiID id_INFORMATION_RENDER_FRAME = window->GetID("##id_INFORMATION_RENDER_FRAME");
		ImGui::ItemSize(bb, style.FramePadding.y);
		ImGui::ItemAdd(bb, id_INFORMATION_RENDER_FRAME);

		bool fir, sec;
		ImGui::ButtonBehavior(bb, id_INFORMATION_RENDER_FRAME, &fir, &sec, NULL);

		ImVec2 text_pos = ImVec2(
			bb.Min.x + main_button_size.x / 2 - txt_size.x / 2,
			bb.Min.y + main_button_size.y / 2 - txt_size.y / 2);
		ImGui::RenderText(text_pos, text_info.c_str());
		ImGui::PopFont();
		ImGui::SetWindowFontScale(1.f);



		if (show != nullptr) {

			std::string name    = tr("startupWindow.information.name");
			std::string desc    = tr("startupWindow.information.description");
			std::string created = tr("startupWindow.information.created");
			std::string edited  = tr("startupWindow.information.edited");
			std::string spented = tr("startupWindow.information.spented");
			std::string lines   = tr("startupWindow.information.lines");


			UTF8_SPLITER_ERROR error;

			

			int max_length = (std::max)({
				count_symbols_utf8(name, error),
				count_symbols_utf8(desc, error),
				count_symbols_utf8(created, error),
				count_symbols_utf8(edited, error),
				count_symbols_utf8(spented, error),
				count_symbols_utf8(lines, error)
			});

			PadRight_UTF8(name,    ' ', max_length + 1);
			PadRight_UTF8(desc,    ' ', max_length + 1);
			PadRight_UTF8(created, ' ', max_length + 1);
			PadRight_UTF8(edited,  ' ', max_length + 1);
			PadRight_UTF8(spented, ' ', max_length + 1);
			PadRight_UTF8(lines,   ' ', max_length + 1);

			name	+= ": ";
			desc	+= ": ";
			created += ": ";
			edited	+= ": ";
			spented += ": ";
			lines	+= ": ";




			ImGui::TextColored(style.Colors[ImGuiCol_TextDisabled], name.c_str());
			ImGui::SameLine(0);
			ImGui::Text(show->name.c_str());
			ImGui::Separator();

			ImGui::TextColored(style.Colors[ImGuiCol_TextDisabled], desc.c_str());
			ImGui::SameLine(0);
			ImGui::TextWrapped(show->description.c_str());
			ImGui::Separator();

			ImGui::TextColored(style.Colors[ImGuiCol_TextDisabled], created.c_str());
			ImGui::SameLine(0);
			ImGui::Text(timestamp_to_DateAndTime_str(show->timeCreated).c_str());
			ImGui::Separator();

			ImGui::TextColored(style.Colors[ImGuiCol_TextDisabled], edited.c_str());
			ImGui::SameLine(0);
			ImGui::Text(timestamp_to_DateAndTime_str(show->timeLastChange).c_str());
			ImGui::Separator();

			ImGui::TextColored(style.Colors[ImGuiCol_TextDisabled], spented.c_str());
			ImGui::SameLine(0);
			ImGui::Text(timestamp_to_Time_str(show->timeSpent).c_str());
			ImGui::Separator();

			ImGui::TextColored(style.Colors[ImGuiCol_TextDisabled], lines.c_str());
			ImGui::SameLine(0);
			ImGui::Text(std::to_string(show->countLines).c_str());

			ImGui::SeparatorText(tr("startupWindow.information.files").c_str());

			for (int i = 0; i < show->files.size(); i++) {
				if (show->files[i] == show->mainFile){
					ImGui::Text(ICON_FA_CIRCLE_DOT);
				}
				else {
					ImGui::Dummy(ImGui::CalcTextSize(ICON_FA_CIRCLE_DOT));
				}
				ImGui::SameLine();
				ImGui::Text(ICON_FA_FILE);
				ImGui::SameLine();


				const std::string BasePathToSource = "source\\";

				std::string OutputFile = show->files[i];

				auto find_pos = OutputFile.find(BasePathToSource);
				if (find_pos != std::string::npos){
					OutputFile.erase(find_pos, BasePathToSource.size());
				}

				
				ImGui::Text(OutputFile.c_str());
			}

		}
		else {
			ImGui::TextWrapped(tr("startupWindow.information.tooltip").c_str());
		}

		ImGui::EndChild();


		ImGui::End();
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();


	Update();
}

void StartupWindow::Update() {

	if (Index_pressed_main != -1) {
		TryingOpen(last_solutions->at(Index_pressed_main).path2file);
	}
	else if (Index_pressed_fav != -1) {
		last_solutions->at(Index_pressed_fav).isFavorite = !last_solutions->at(Index_pressed_fav).isFavorite;

		SaveSolution(last_solutions->at(Index_pressed_fav));

		std::sort(
			last_solutions->begin(),
			last_solutions->end(),
			[](const SolutionInfo& a, const SolutionInfo& b)
			{
				if (a.isFavorite == b.isFavorite)
					return a.timeLastChange > b.timeLastChange;
				else
					return a.isFavorite > b.isFavorite;

			});
	}
	else if (Index_pressed_del != -1) {
		last_solutions->erase(last_solutions->begin() + Index_pressed_del);
		CurrentIndexHovered = -1;

	}


	Index_pressed_main = -1;
	Index_pressed_fav = -1;
	Index_pressed_del = -1;
}

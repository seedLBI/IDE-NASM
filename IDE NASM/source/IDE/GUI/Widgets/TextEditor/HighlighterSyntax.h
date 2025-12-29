#ifndef HIGHLIGHTER_SYNTAX_H
#define HIGHLIGHTER_SYNTAX_H


#include "ThirdParty/ImGui/imgui.h"
#include "Utils/Text/Utils.Text.h"
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "IDE/Core/Theme/interface/IThemeLoadable.h"

enum HighlighterSyntaxType {
	Highlighter_Label,
	Highlighter_Instruction,
	Highlighter_AssemblerDirective,
	Highlighter_PreprocessorDirective,
	Highlighter_PredefinedMacros,
	Highlighter_Section,
	Highlighter_Register_AVX,
	Highlighter_Register_MMX,
	Highlighter_Register_SSE,
	Highlighter_Register_Control,
	Highlighter_Register_Debug,
	Highlighter_Register_FloatingPoint,
	Highlighter_Register_General,
	Highlighter_UNKNOWN
};

class HighlighterSyntax : public IThemeLoadable {
public:

	static HighlighterSyntax& Instance();

	std::string MakeRegex_Label();

	std::string MakeRegex_Instruction();
	std::string MakeRegex_PreprocessorDirectives();
	std::string MakeRegex_PredefinedMacros();
	std::string MakeRegex_AssemblerDirectives();
	std::string MakeRegex_Sections();

	std::string MakeRegex_Register_General();
	std::string MakeRegex_Register_Control();
	std::string MakeRegex_Register_Debug();
	std::string MakeRegex_Register_FloatingPoint();
	std::string MakeRegex_Register_AVX();
	std::string MakeRegex_Register_MMX();
	std::string MakeRegex_Register_SSE();

	std::vector<std::string> GetAllWords();


	void LoadColors() override;
	std::vector<NamedColor> GetDefaultLightColors() override;
	std::vector<NamedColor> GetDefaultDarkColors() override;


	
	ImVec4					 GetColorFromType(const HighlighterSyntaxType& index);
	HighlighterSyntaxType    GetTypeFromText (const std::string& text);

	void DrawTooltip(const std::string& text);


private:
	HighlighterSyntax();
	~HighlighterSyntax();

	HighlighterSyntax(HighlighterSyntax const&) = delete;
	HighlighterSyntax& operator= (HighlighterSyntax const&) = delete;


	nlohmann::json json_instructions;
	nlohmann::json json_syntax;

	ImVec4 color_UNKNOWN;

	ImVec4 color_Label;
	ImVec4 color_Instruction;
	ImVec4 color_PreprocessorDirectives;
	ImVec4 color_PredefinedMacros;
	ImVec4 color_AssemblerDirectives;
	ImVec4 color_Sections;

	ImVec4 color_Register_MMX;
	ImVec4 color_Register_AVX;
	ImVec4 color_Register_SSE;
	ImVec4 color_Register_Control;
	ImVec4 color_Register_Debug;
	ImVec4 color_Register_FloatingPoint;
	ImVec4 color_Register_General;

	inline std::string MakeRegex_LowercaseAll(const nlohmann::json& array_json);
	inline std::string MakeRegex(const nlohmann::json& array_json);

	void ReadJSONFromFile();


	void DrawTooltipInstruction(const nlohmann::json& json_data);
	void DrawTooltipSimpleInfo(const nlohmann::json& json_data);
	void DrawTooltipRegister(const nlohmann::json& json_data);


};






#endif
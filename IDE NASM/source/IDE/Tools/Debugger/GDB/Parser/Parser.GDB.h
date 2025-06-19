#ifndef PARSER_GDB_H
#define PARSER_GDB_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <stdexcept>
#include <cctype>
#include <nlohmann/json.hpp>

class ParserGDB
{
public:
    static nlohmann::json parse(const std::string& input);

private:




    static nlohmann::json parse_output(const std::string& s, size_t& pos);

    struct disasembly_block {
        std::string adress;
        std::string symbol;
        std::string instruction;
        long long disp;
        bool current;
    };

    static disasembly_block parse_disasm_line(const std::string& line);

    static nlohmann::json extract_disasm_blocks(const std::string& all);

    static nlohmann::json extract_registers(const std::string& all);


    static nlohmann::json parse_record(const std::string& s, size_t& pos);

    static std::string record_type_name(char c);

    static long long parse_token(const std::string& s, size_t& pos);

    static nlohmann::json parse_payload(const std::string& s, size_t& pos);

    static nlohmann::json parse_result_list(const std::string& s, size_t& pos);

    static std::string parse_identifier(const std::string& s, size_t& pos);

    static nlohmann::json parse_value(const std::string& s, size_t& pos);

    static nlohmann::json parse_c_string(const std::string& s, size_t& pos);

    static char decode_escape(char c);

    static nlohmann::json parse_tuple(const std::string& s, size_t& pos);

    static nlohmann::json parse_list(const std::string& s, size_t& pos);

    static void expect(const std::string& s, size_t& pos, char c);
};




#endif
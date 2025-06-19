#include "Parser.GDB.h"

nlohmann::json ParserGDB::parse(const std::string& input) {
    size_t pos = 0;
    auto records = parse_output(input, pos);

    std::string console_all;
    for (auto& rec : records) {
        if (rec["record_type"] == "console-stream") {
            console_all += rec["message"].get<std::string>();
        }
    }

    auto disasm_blocks = extract_disasm_blocks(console_all);
    if (!disasm_blocks.empty()) {
        return disasm_blocks;
    }

    auto regs = extract_registers(console_all);
    if (!regs.empty()) {
        return regs;
    }

    return records;

}

ParserGDB::disasembly_block ParserGDB::parse_disasm_line(const std::string& line) {
    static const std::regex rx(
        R"(^\s*(\=\>)?\s*([0-9a-fx]+)\s+<([^+>]+)\+(\d+)>:\s*(.+)$)"
    );
    std::smatch m;
    if (std::regex_match(line, m, rx)) {


        disasembly_block data;

        data.adress = m[2].str();
        data.symbol = m[3].str();
        data.disp = std::stoll(m[4].str());
        data.instruction = m[5].str();
        data.current = m[1].matched;
        
        return data;

    }
    return {};
}

nlohmann::json ParserGDB::extract_disasm_blocks(const std::string& all) {
    std::vector<nlohmann::json> blocks;
    static const std::regex block_rx(
        R"(Dump of assembler code[\s\S]*?End of assembler dump\.)"
    );
    auto it = std::sregex_iterator(all.begin(), all.end(), block_rx);
    auto end = std::sregex_iterator();

    std::string current_adress;
    std::string first_adress;
    std::string last_adress;

    
    std::vector<disasembly_block> lines;

    for (; it != end; ++it) {
        std::string block = it->str();

        std::istringstream ss(block);
        std::string ln;

        std::getline(ss, ln);
        while (std::getline(ss, ln)) {
            if (ln.find("End of assembler dump.") != std::string::npos)
                break;
            auto jl = parse_disasm_line(ln);

            if (jl.adress.empty() == false) {

                if (first_adress.empty()){
                    first_adress = jl.adress;
                }
                else
                    if (jl.current)
                        current_adress = jl.adress;

                lines.emplace_back(jl);
            }



        }
    }

    nlohmann::json result;

    if (lines.empty() == false) {
        result["from"] = lines.front().adress;
        result["to"] = lines.back().adress;
        result["current"] = current_adress;


        uint64_t first_adr = strtoull(lines.front().adress.c_str(), NULL, 16);


        for (int i = 0; i < lines.size(); i++) {
            result["addr"][lines[i].adress]["offset"] = strtoull(lines[i].adress.c_str(), NULL, 16) - first_adr;
            result["addr"][lines[i].adress]["symbol"] = lines[i].symbol;
            result["addr"][lines[i].adress]["disp"] = lines[i].disp;
            result["addr"][lines[i].adress]["instr"] = lines[i].instruction;
        }

    }



    return result;
}

nlohmann::json ParserGDB::extract_registers(const std::string& all) {
    std::istringstream ss(all);
    std::string line;

    nlohmann::json regs = nlohmann::json::object();
    std::regex reg_line(R"(^\s*([a-z0-9]+)\s+(.+)$)");
    std::smatch m;

    while (std::getline(ss, line)) {
        if (!std::regex_match(line, m, reg_line))
            continue;
        std::string name = m[1].str();
        std::string rest = m[2].str();

        // General purpose and segment registers: hex + dec
        std::regex gd(R"(^0x([0-9a-fA-F]+)\s+(\d+).*$)");
        std::regex flags_rx(R"(^0x([0-9a-fA-F]+)\s+\[(.+)\].*$)");
        std::regex raw_rx(R"(raw\s+0x([0-9a-fA-F]+))");
        std::smatch sub;

        if (std::regex_search(rest, sub, raw_rx)) {
            // Floating point registers st0-st7
            regs[name] = {
                {"raw", std::string("0x") + sub[1].str()}
            };
        }
        else if (name.rfind("ymm", 0) == 0) {
            // SIMD registers: keep full struct
            regs[name] = rest;
        }
        else if (std::regex_search(rest, sub, flags_rx)) {
            // Flags registers eflags, mxcsr
            std::string hexv = std::string("0x") + sub[1].str();
            // split flags by space
            std::istringstream fs(sub[2].str());
            std::string flag;
            nlohmann::json arr = nlohmann::json::array();
            while (fs >> flag) arr.push_back(flag);
            regs[name] = {
                {"hex", hexv},
                {"flags", arr}
            };
        }
        else if (std::regex_search(rest, sub, gd)) {
            // general registers
            std::string hexv = std::string("0x") + sub[1].str();
            long long decv = std::stoll(sub[2].str());
            regs[name] = {
                {"hex", hexv},
                {"dec", decv}
            };
        }
    }

    nlohmann::json out;
    if (!regs.empty()) out["registers"] = regs;
    return out;
}

nlohmann::json ParserGDB::parse_output(const std::string& s, size_t& pos) {
    nlohmann::json result = nlohmann::json::array();
    while (pos < s.size()) {
        // Skip newlines
        if (s[pos] == '\n' || s[pos] == '\r') { ++pos; continue; }
        if (s[pos] == '(') {
            // consume until end of line
            while (pos < s.size() && s[pos] != '\n') ++pos;
            continue;
        }
        result.push_back(parse_record(s, pos));
    }
    return result;
}

nlohmann::json ParserGDB::parse_record(const std::string& s, size_t& pos) {
    char type = s[pos++];
    nlohmann::json rec;
    rec["record_type"] = record_type_name(type);

    // Parse optional token
    long long token = parse_token(s, pos);
    if (token >= 0) rec["token"] = token;

    // Parse result-class and optional fields or stream message
    if (type == '^' || type == '*' || type == '+' || type == '=') {
        rec["payload"] = parse_payload(s, pos);
    }
    else if (type == '~' || type == '@' || type == '&') {
        rec["message"] = parse_c_string(s, pos);
    }
    else {
        throw std::runtime_error(std::string("Unknown record type: ") + type);
    }

    // Consume trailing newline
    if (pos < s.size() && (s[pos] == '\r' || s[pos] == '\n')) ++pos;
    return rec;
}

std::string ParserGDB::record_type_name(char c) {
    switch (c) {
    case '^': return "result";
    case '*': return "exec-async";
    case '+': return "status-async";
    case '=': return "notify-async";
    case '~': return "console-stream";
    case '@': return "target-stream";
    case '&': return "log-stream";
    default:  return "unknown";
    }
}

long long ParserGDB::parse_token(const std::string& s, size_t& pos) {
    size_t start = pos;
    while (pos < s.size() && isdigit(s[pos])) ++pos;
    if (start == pos) return -1;
    return std::stoll(s.substr(start, pos - start));
}

nlohmann::json ParserGDB::parse_payload(const std::string& s, size_t& pos) {
    // Parse result-class (identifier)
    std::string cls = parse_identifier(s, pos);
    nlohmann::json obj = nlohmann::json::object();
    obj["class"] = cls;
    // If there are fields, parse after comma
    if (pos < s.size() && s[pos] == ',') {
        ++pos;
        nlohmann::json fields = parse_result_list(s, pos);
        for (auto& it : fields.items()) {
            obj[it.key()] = it.value();
        }
    }
    return obj;
}

nlohmann::json ParserGDB::parse_result_list(const std::string& s, size_t& pos) {
    nlohmann::json obj = nlohmann::json::object();
    while (pos < s.size() && s[pos] != '\n' && s[pos] != '\r') {
        std::string key = parse_identifier(s, pos);
        expect(s, pos, '=');
        nlohmann::json val = parse_value(s, pos);
        obj[key] = val;
        if (pos < s.size() && s[pos] == ',') {
            ++pos;
            continue;
        }
        break;
    }
    return obj;
}

std::string ParserGDB::parse_identifier(const std::string& s, size_t& pos) {
    size_t start = pos;
    while (pos < s.size() && (isalnum(s[pos]) || s[pos] == '-')) ++pos;
    return s.substr(start, pos - start);
}

nlohmann::json ParserGDB::parse_value(const std::string& s, size_t& pos) {
    if (s[pos] == '"') return parse_c_string(s, pos);
    if (s[pos] == '{') return parse_tuple(s, pos);
    if (s[pos] == '[') return parse_list(s, pos);
    // bare identifier or number
    std::string id = parse_identifier(s, pos);
    return id;
}

nlohmann::json ParserGDB::parse_c_string(const std::string& s, size_t& pos) {
    ++pos; // skip '"'
    std::string out;
    while (pos < s.size()) {
        if (s[pos] == '"') { ++pos; break; }
        if (s[pos] == '\\' && pos + 1 < s.size()) {
            out += decode_escape(s[++pos]);
            ++pos;
        }
        else {
            out += s[pos++];
        }
    }
    return out;
}

char ParserGDB::decode_escape(char c) {
    switch (c) {
    case 'n':  return '\n';
    case 't':  return '\t';
    case 'r':  return '\r';
    case '"': return '"';
    case '\\':return '\\';
    default:   return c;
    }
}

nlohmann::json ParserGDB::parse_tuple(const std::string& s, size_t& pos) {
    ++pos; // skip '{'
    nlohmann::json obj = parse_result_list(s, pos);
    expect(s, pos, '}');
    return obj;
}

nlohmann::json ParserGDB::parse_list(const std::string& s, size_t& pos) {
    ++pos; // skip '['
    nlohmann::json arr = nlohmann::json::array();
    while (pos < s.size() && s[pos] != ']') {
        if (s[pos] == '{')      arr.push_back(parse_tuple(s, pos));
        else if (s[pos] == '[') arr.push_back(parse_list(s, pos));
        else if (s[pos] == '"')arr.push_back(parse_c_string(s, pos));
        else {
            std::string id = parse_identifier(s, pos);
            arr.push_back(id);
        }
        if (s[pos] == ',') ++pos;
    }
    expect(s, pos, ']');
    return arr;
}

void ParserGDB::expect(const std::string& s, size_t& pos, char c) {
    if (pos >= s.size() || s[pos] != c) {
        throw std::runtime_error(std::string("Expected '") + c + "' at position " + std::to_string(pos));
    }
    ++pos;
}



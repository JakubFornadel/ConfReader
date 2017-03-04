#include "IniParser.h"
#include <fstream>
#include <regex>

IniParser::IniParser() : default_section_("Default") {
}

IniParser * IniParser::Clone() const {
  return new IniParser(*this);
}

void IniParser::ResetData() {
  for (auto & inner_storage : params_) {
    inner_storage.second.clear();
  }
  params_.clear();
}

void IniParser::LoadAndParse(std::string file_path) {
  std::ifstream ifs(file_path);

  if (!ifs.is_open()) {
    throw std::runtime_error("Unable to open file: " + file_path);
  }

  ParseLines(ifs);

  if (ifs.bad()) {
    throw std::runtime_error("Error while reading file: " + file_path);
  }

  ifs.close();
}

void IniParser::ParseLines(std::ifstream & ifs) {
  // Regular expressions, which define supported line formats, all other formats are ignored
  // Supported comment format -> ; this is comment
  std::regex comment_regex("^[\\s]*;(.*)");
  // Supported param format -> param_name = param_value
  // Restrictions: no whitespace in param_name and param_value, both must contain at least 1 char
  std::regex param_regex("^[\\s]*([^\\s]+)[\\s]*=[\\s]*([^\\s]+)[\\s]*");
  // Supported quoted param format -> param_name = "param_value"
  // Restrictions: no whitespace in param_name, param_name must contain at least 1 char,
  //               param_value can contain whitespaces, param_value == "" is accepted as NULL
  std::regex quoted_param_regex("^[\\s]*([^\\s]+)[\\s]*=[\\s]*\\\"(.*)\\\"[\\s]*");
  // Supported section format -> "  [ section_name ]   ".
  // Restrictions: no whitespace in section_name, only one '[' and ']' per line
  std::regex section_regex("^[\\s]*\\[[\\s]*([^\\[\\]\\s]+)[\\s]*\\][\\s]*");
  // Supported Empty line
  std::regex empty_line_regex("^[\\s]*");

  std::smatch comment_match;
  std::smatch empty_line_match;
  std::smatch param_match;
  std::smatch section_match;

  // Parse all lines using regex's and store found parameters
  // set actual section to default in case ini file contains parameters without specified section
  std::string actual_section = default_section_;
  std::string param_name, param_value;
  std::string line;
  while(std::getline(ifs, line)) {
    // Quoted + basic param
    if (std::regex_match(line, param_match, quoted_param_regex) ||
        std::regex_match(line, param_match, param_regex)) {
      if (param_match.size() == 3) {
        param_name = param_match[1];
        param_value = param_match[2];
        params_[actual_section][param_name] = param_value;
      }
    }
    // Section
    else if (std::regex_match(line, section_match, section_regex)) {
      if (section_match.size() == 2) {
        actual_section = section_match[1];
      }
    }
    // Comment + empty lines - ignore
    else if (std::regex_match(line, comment_match, comment_regex) ||
             std::regex_match(line, empty_line_match, empty_line_regex)) {
    }
    // Invalid line formats
    else {
      throw std::runtime_error("Found invalid line format while parsing.\nLine: " + line );
    }
  }
}

std::tuple<std::string, std::string> IniParser::ValidateTokens(const std::vector<std::string>& tokens) const {
  size_t tokens_num = tokens.size();

  // For .ini file, allow max two nested parameter tokens, one for section and one for param name
  switch (tokens_num) {
    case 1:
      return std::make_tuple(default_section_ /* section */, tokens[0] /* param */);
    case 2:
      return std::make_tuple(tokens[0] /* section */, tokens[1] /* param */);
    default:
      throw std::runtime_error("Invalid param_path. Max. number of nested parameter tokens is 2. Actual: "
                              + std::to_string(tokens_num));
  }
}

std::string IniParser::GetParam(std::vector<std::string>& param_tokens) const {
  std::string section, param;

  // Validate tokens and get values of section and param
  auto valid_tokens = ValidateTokens(param_tokens);
  section = std::get<0 /* section */>(valid_tokens);
  param = std::get<1 /* param */>(valid_tokens);

  // Check if such parameter exist
  if (Has(param_tokens) == false) {
    throw std::out_of_range("Invalid parameter: " + section + "." + param);
  }

  return params_.at(section).at(param);
}

bool IniParser::Has(std::vector<std::string>& param_tokens) const {
  std::string section, param;

  // Validate tokens and get values of section and param
  auto valid_tokens = ValidateTokens(param_tokens);
  section = std::get<0 /* section */>(valid_tokens);
  param = std::get<1 /* param */>(valid_tokens);

  // Check if specified section and param exist
  if (params_.find(section) == params_.end()) {
    return false;
  }

  const auto& section_params = params_.at(section);
  if (section_params.find(param) == section_params.end()) {
    return false;
  }

  return true;
}

Parser::ParsStorage IniParser::GetAllParams() const {
  ParsStorage ret_params;

  for (const auto & inner_storage : params_) {
    // Add also section name
    ret_params.push_back(std::pair<std::string, std::string>(inner_storage.first, "section_name"));

    for (const auto & sample : inner_storage.second) {
      ret_params.push_back(sample);
    }
  }

  return ret_params;
}


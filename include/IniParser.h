#ifndef INIPARSER_H
#define INIPARSER_H

#include "Parser.h"
#include <unordered_map>

// Specific Parser for *.ini files
class IniParser : public Parser {
public:
  IniParser();
  IniParser(const IniParser&) = default;
  IniParser& operator = (const IniParser&) = default;

  IniParser(IniParser&&) = default;
  IniParser& operator = (IniParser&&) = default;

  virtual ~IniParser() = default;

  virtual void ResetData();
  virtual IniParser * Clone() const override;
  virtual void LoadAndParse(std::string file_path) override;
  virtual std::string GetParam(std::vector<std::string>& param_tokens) const override;
  virtual bool Has(std::vector<std::string>& param_tokens) const override;
  virtual Parser::ParsStorage GetAllParams() const override;
private:
  // Data type of storage for parsed parameters from *.ini files
  using IniParsStorage = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;
  IniParsStorage params_;

  // Default section name for parameters without specified section inside .ini file
  std::string default_section_;

  // Parses lines and stores found parameters
  void ParseLines(std::ifstream & ifs);

  // Validates input tokens, return valid values
  std::tuple<std::string, std::string> ValidateTokens(const std::vector<std::string>& tokens) const;
};

#endif // INIPARSER_H

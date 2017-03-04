#ifndef CONFREADER_H
#define CONFREADER_H

#include "Parser.h"
#include <memory>
#include <vector>
#include <sstream>

class ConfReader {
public:
  ConfReader(Parser * parser);
  ConfReader(std::unique_ptr<Parser> parser);
  ConfReader(const ConfReader& other);
  ConfReader& operator=(const ConfReader& other);

  ConfReader(ConfReader&&) = default;
  ConfReader& operator=(ConfReader&&) = default;
  virtual ~ConfReader() = default;

  // Forwards file_path to parser, which will open, parse it and store found parameters
  void LoadFile(const std::string& file_path);

  // Checks existence of parameter, returns true if it does
  bool Has(const std::string& param_path, const char delimiter = '.') const;

  // Returns value of specified parameter by its path - Section1.param,
  // where Sections represent nested groups of parameters
  std::string GetParam(const std::string& param_path, const char delimiter = '.') const;

  // GetParam method with specified return type
  template <typename T>
  T GetParam(const std::string& param_path, const char delimiter = '.') const;

  // Returns all stored parameters,
  // in order as: section1, section1_par1, section1_par2, section2, section2_par1, ...
  Parser::ParsStorage GetAllParams();

  // Deletes stored data
  void Reset();

  // Change parser, delete already stored data
  void ChangeParser(std::unique_ptr<Parser> parser);
private:
  // Pointer to abstract base class of Parser,
  // possible to switch between different parsers -> different file formats, ...
  std::unique_ptr<Parser> parser_;

  // Splits param_path into tokens
  std::vector<std::string> SplitParamTokens(const std::string& param_path, const char delimiter) const;
};

template <typename T>
T ConfReader::GetParam(const std::string& param_path, const char delimiter) const {
  // Gets parameter string value
  auto param_tokens = SplitParamTokens(param_path, delimiter);
  std::string par_value = parser_->GetParam(param_tokens);

  // Converts it to specified type
  std::stringstream ss(par_value);
  T converted_par;
  ss >> converted_par;

  if (ss.fail()) {
    throw std::runtime_error("Unable to convert param: " + par_value + " from std::string to " + typeid(converted_par).name());
  }

  return converted_par;
}

#endif // CONFREADER_H

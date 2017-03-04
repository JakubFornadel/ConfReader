#include "ConfReader.h"
#include <exception>
#include <iostream>
#include <sstream>

ConfReader::ConfReader(Parser * parser) : parser_(parser) {
}

ConfReader::ConfReader(std::unique_ptr<Parser> parser) {
  parser_ = std::move(parser);
}

ConfReader::ConfReader(const ConfReader& other) {
  parser_ = std::unique_ptr<Parser>(other.parser_.get()->Clone());
}

ConfReader& ConfReader::operator = (const ConfReader& other) {
  parser_.reset();
  parser_ = std::unique_ptr<Parser>(other.parser_.get()->Clone());

  return *this;
}

void ConfReader::LoadFile(const std::string & file_path) {
  if (parser_ == nullptr) {
    throw std::runtime_error("Trying to use ConfReader class without specified Parser");
  }

  parser_->LoadAndParse(file_path);
}

bool ConfReader::Has(const std::string& param_path, const char delimiter) const {
  auto param_tokens = SplitParamTokens(param_path, delimiter);

  return parser_->Has(param_tokens);
}

Parser::ParsStorage ConfReader::GetAllParams() {
  return parser_->GetAllParams();
}

std::string ConfReader::GetParam(const std::string& param_path, const char delimiter) const {
  auto param_tokens = SplitParamTokens(param_path, delimiter);

  return parser_->GetParam(param_tokens);
}

std::vector<std::string> ConfReader::SplitParamTokens(const std::string& param_path, const char delimiter) const {
  std::string token;
  std::vector<std::string> tokens;

  std::stringstream ss(param_path);
  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(std::move(token));
  }

  // If param_path is simple(not nested by delimiter) param, add it
  if (tokens.empty() == true) {
    tokens.push_back(param_path);
  }

  return tokens;
}

void ConfReader::Reset() {
  parser_->ResetData();
}

void ConfReader::ChangeParser(std::unique_ptr<Parser> parser) {
  parser_.reset();
  parser_ = std::move(parser);
}

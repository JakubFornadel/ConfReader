#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <memory>
#include <vector>

// Abstract Parser interface
class Parser {
public:
    Parser() = default;
    Parser(const Parser&) = default;
    Parser& operator = (const Parser&) = default;

    Parser(Parser&&) = default;
    Parser& operator = (Parser&&) = default;

    virtual ~Parser() = default;

    // Returns new class instance with the same data
    virtual Parser * Clone() const = 0;

    // Deletes stored data
    virtual void ResetData() = 0;

    // Loads file, parses it and stores found parameters
    virtual void LoadAndParse(const std::string& file_path) = 0;

    // Returns value of specified parameter by tokens, e.g. ("Section1", "Section2", "param_name")
    virtual std::string GetParam(std::vector<std::string>& param_tokens) const = 0;

    // Checks existence of parameter, returns true if it does
    virtual bool Has(std::vector<std::string>& param_tokens) const = 0;

    // Returns all stored parameters as ParsStorage type,
    // keeps order as: section1, section1_par1, section1_par2, section2, section2_par1, ...
    using ParsStorage = std::vector<std::pair<std::string, std::string>>;
    virtual ParsStorage GetAllParams() const = 0;
private:
};

#endif // PARSER_H

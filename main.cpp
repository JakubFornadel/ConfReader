#include <iostream>

#include "ConfReader.h"
#include "IniParser.h"

int main() {
  try {
    ConfReader config(std::make_unique<IniParser>());
    config.LoadFile("test_file.ini");

    std::cout << config.GetParam("personal_info.name") << std::endl;
    std::cout << config.GetParam<int>("personal_info.age") << std::endl;

    // Load second configuration file, override existing parameters
    config.LoadFile("test_file2.ini");
    std::cout << config.GetParam<int>("personal_info.name") << std::endl;


    // Loop through all parameters
    for (auto & sample : config.GetAllParams()) {
      std::cout << sample.first << " : " << sample.second << std::endl;
      // sample.second == std::string("section_name") -> begining of "sample.first" section parameters
    }


    // Delete previously stored data and load new file
    config.Reset();
    config.LoadFile("test_file3.ini");


    // Check if parameter exist before accessing it - "exception safe"
    if (config.Find("personal_info.age") == true) {
      auto param = config.GetParam<int>("personal_info.age");
      // Do something with parameter...
    }


    // Optional functionality - Read different configuration file format(.csv) and load file
    // config.ChangeParser(std::make_unique<CsvParser>());
    // config.LoadFile("test_file.csv");

  } catch (const std::exception & e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}

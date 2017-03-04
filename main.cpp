#include <iostream>

#include "ConfReader.h"
#include "IniParser.h"

int main() {
  try {
    // Creates ConfReader class instance with IniParser as parameter
    ConfReader config(std::make_unique<IniParser>());
    // Loads file and stores found parameters
    config.LoadFile("./tests/files/test_file.ini");

    // Gets parameter as string, throws an exception if it does not exist
    std::cout << config.GetParam("personal_info.name") << std::endl;
    // Gets parameter with specified return type, throws an exception if conversion is not possible
    std::cout << config.GetParam<int>("personal_info.age") << std::endl << std::endl;


    // Loads second configuration file, override existing parameters
    config.LoadFile("./tests/files/test_file2.ini");
    // Checks if parameter exist before accessing it - "exception safe"
    if (config.Find("personal_info.age") == true) {
      auto param = config.GetParam<int>("personal_info.age");
      // Do something with parameter...
    }


    // Loops through all parameters
    for (auto & sample : config.GetAllParams()) {
      std::cout << sample.first << " : " << sample.second << std::endl;
      // sample.second == std::string("section_name") -> begining of "sample.first" section parameters
    }


    // Deletes previously stored data and load new file
    config.Reset();
    config.LoadFile("./tests/files/test_file3.ini");


    // Optional functionality - Read different configuration file format(.csv) and load file
    // config.ChangeParser(std::make_unique<CsvParser>());
    // config.LoadFile("test_file.csv");

  } catch (const std::exception & e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}

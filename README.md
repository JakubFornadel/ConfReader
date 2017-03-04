# ConfReader

###Overview###
ConfReader is C++ class, which allows programmers to easily access configuration files in different formats. For proper working of the class must be specified FileParser, which implement functions for parsing file, etc... ConfReader provides interface for adding/switching between parsers without needing to do changes to the class. In examples below is used Parser for files in .ini format.

###Supported *.ini file format###
```INI
; This is sample configuration file
; Comment start with ';', comments are not allowed on the same line as parameters or sections

; Supported line formats:
    ; param_name = param_value
    ; Restrictions: no whitespace in param_name and param_value,
    ;               both must contain at least 1 char

    ; param_name = "param_value"
    ; Restrictions: no whitespace in param_name, param_name must contain at least 1 char,
    ;               param_value can contain whitespaces, param_value "" is accepted as NULL

    ; [ section_name ]
    ; Restrictions: no whitespace in section_name, only one '[' and ']' per line

    ; empty line

; Parameters with no section
file_name = "test_file.ini"
date = "Friday March 3,2017"

[ personal_info ]
name = "Jakub Fornadel"
age = 25
brother = Michal
sister = ""

[work_info]
programming_language=C++
position=Developer
```

###Usage###
Main interface is defined in ConfReader.h. This is simple example of usage: 

```C++
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
```

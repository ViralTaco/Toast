/*
 * Copyright (c) 2018 Capobianco A.
 * SPDX-License-Identifier: MIT 
 * <http://www.opensource.org/licenses/MIT>
 */

#include "Toaster.hpp"

#include <cstring>

namespace view {
const std::string kVersion{
  "toast Copyright (c) 2018 ViralTaco\nVersion 2.0.1 (https://github.com/ViralTaco/Toast)\n"
};
const std::string kHelpMsg{
  "toast [make <string>] [cut <number>] [done <number>] [clear]\n\n"
  "make <string>    Makes a toast \n"
  "                 ie: toast make \"Make a new toast\"\n"
  "do <string>      Same as make\n"
  "cut <ID>         Removes a toast from the list\n"
  "                 ie: toast cut 1\n"
  "rm <ID>          Same as cut\n"
  "done <ID>        Check the box of the toast ID\n"
  "                 ie: toast done 2\n"
  "clear            Removes everything from the list\n"
  "version          Show version\n"
  "help             Show this help message\n"
};
const std::string kUsage{
    "usage: toast [make <string>] [cut <number>] [done <number>] [clear]\n"
};

[[noreturn]] void print_usage() {
  std::cout << kUsage;
  std::exit(EXIT_FAILURE);
}
} // namespace view

int main(int argc, char **argv) {
  using namespace view;
  if (strcmp(std::getenv("USER"), "root") == 0) {
    std::cerr << "Toast: error: cannot run as root\n";
    return EXIT_FAILURE;
  }

  Toaster toast(std::getenv("HOME"), ".toast");
  bool is_success = false;
  if (argc > 1) {
    const std::string& command = argv[1];
    
    if (command == "version" or command == "--version") {
      std::cout << kVersion;
      return EXIT_SUCCESS;
    } else if (command == "help" or command == "--help") {
      std::cout << kHelpMsg;
      return EXIT_SUCCESS;
    } else if (command == "clear") {
      while (true) {
        std::string answer{""};
        std::cout << "Are you sure? (y/n) ";
        std::cin >> answer;
        if (answer == "yes" or answer == "Yes") {
          toast.clear();
          return EXIT_SUCCESS;
        } else if (answer == "y" or answer == "Y") {
          std::cout << "Please type \"yes\"\n";
        } else {
          std::cout << "Well make up your mind, you got me excited :(\n";
          return EXIT_SUCCESS;
        }
      }
    } else if (argv[2] != nullptr) { 
      if (command == "make" or command == "do") {
        is_success = toast.add(argv[2]);
      } else {
        size_t id;
        try {
          id = std::stoul(argv[2]);
        } catch (std::invalid_argument) {
          
        }  
        if (command == "cut" or command == "rm") {
          is_success = toast.rm(id);
        } else if (command == "done") {
          is_success = toast.check(id);
        }
      }
    }
  }
  
  if (is_success or argc == 1) {
    std::cout << toast;
  } else {
    print_usage();
  }
}
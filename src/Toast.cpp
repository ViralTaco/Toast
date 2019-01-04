/*
 * Copyright (c) 2018 - 2019 Capobianco A.
 * SPDX-License-Identifier: MIT 
 * <http://www.opensource.org/licenses/MIT>
 */

#include "headers/Toaster.hpp"

#include <cstring>

namespace view {
const std::string kVersion{
  "toast Copyright (c) 2019 Capobianco A. (ViralTaco)\n"
  "Version 2.2.1 (https://github.com/ViralTaco/Toast)\n"
};
const std::string kHelpMsg{
  "toast [make <string>] [cut <ID>] [done <ID>] [print <ID>] [clear]\n\n"
  "make <string>    Makes a toast \n"
  "                 ie: toast make \"Make a new toast\"\n"
  "do <string>      Same as make\n"
  "cut <ID>         Removes a toast from the list\n"
  "                 ie: toast cut 1\n"
  "rm <ID>          Same as cut\n"
  "done <ID>        Check the box of the toast ID\n"
  "                 ie: toast done 2\n"
  "check <ID>       Same as done\n"
  "print <ID>       Prints the toast at the specified ID\n"
  "clear            Removes everything from the list\n"
  "version          Show version\n"
  "help             Show this help message\n"
};
const std::string kUsage{
    "usage: toast [make <string>] [cut <number>] [done <number>] [clear]\n"
};

[[noreturn]] void print_usage() 
{
  std::cout << kUsage;
  std::exit(EXIT_FAILURE);
}

bool is_user_sure()
{
  while (true) {
    std::string answer{""};
    std::cout << "Are you sure? (y/n) ";
    std::cin >> answer;
    if (answer == "yes" or answer == "Yes") {
      return true;
    } else if (answer == "y" or answer == "Y") {
      std::cout << "Please type \"yes\"\n";
    } else {
      break;
    }
  }
  
  std::cout << "Well make up your mind, you got me excited :(\n";
  return false;
}

} // namespace view

int main(int argc, char **argv) 
{
  if (strcmp(std::getenv("USER"), "root") == 0) {
    std::cerr << "Toast: error: cannot run as root\n";
    return EXIT_FAILURE;
  }

  Toaster toast(std::getenv("HOME"), ".toast");
  if (argc == 1) {
    std::cout << toast;
    return EXIT_SUCCESS;
  }
  
  const std::string command = argv[1];
  
  if (argc == 2) {
    if (command == "version" or command == "--version") {
      std::cout << view::kVersion;
    } else if (command == "help" or command == "--help") {
      std::cout << view::kHelpMsg;
    } else if (command == "clear") { 
        if (view::is_user_sure()) toast.clear();
    }
    return EXIT_SUCCESS; 
  }
  
  bool is_success = false;
  size_t id;
  
  if (argc == 3 and argv[2] != nullptr) { 
    if (command == "make" or command == "do") {
      is_success = toast.add(argv[2]);
    } else {
      try {
        id = std::stoul(argv[2]);
      } catch (std::invalid_argument) {
        view::print_usage();
      }  
      
      if (command == "cut" or command == "rm") {
        is_success = toast.rm(id);
      } else if (command == "done" or command == "check") {
        is_success = toast.check(id);
      } else if (command == "print" or command == "echo") {
        std::cout << toast[id];
        return EXIT_SUCCESS;
      }
    }
  }
  
  if (is_success) {
    std::cout << toast;
  } else {
    view::print_usage();
  }
}
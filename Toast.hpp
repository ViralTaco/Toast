/*
 * Copyright (c) 2018 Capobianco A.
 * SPDX-License-Identifier: MIT 
 * <http://www.opensource.org/licenses/MIT>
 */

#include "Toaster.hpp"

class Toast {
private:

  
  Toaster controller_;
  bool is_init_ = false;
  
public:
  Toast(const std::string path, const std::string filename) noexcept(true)
    : controller_(path + "/" + filename) {
  }
  
  void init(std::vector<std::string>& arguments) noexcept(true) {
    is_init_ = true;
    const std::string& command = arguments.at(0);
    
    if (command == "make" or command == "do") {
      
    } else if (command == "cut" or command == "rm") {
      rmtoast(to_size_t(arg_) - 1);
    } else if (command == "done") {
      size_t id = std::stoi(arguments.at(1));
      if (id < controller_.get_count()) {
        controller_.check(id);
      }
    } else {
      print_usage(); 
    }
    
    for (size_t i = 0; i < arguments.size(); ++i);
    
  }
  
  std::string get_help() const noexcept(true) {
    return kHelpMsg_;
  }
  
  std::string get_usage() const noexcept(true) {
    return kUsage_; 
  }
  
  std::string get_current_view() const noexcept(true) {
    return controller_.get_current();
  }
  

};


#if defined (RUN_TESTS)
  std::string help_msg{
    "toast [make <string>] [cut <number>] [done <number>] [clear]\n\n"
    "make <string>    Makes a toast \n"
    "                 ie: toast make \"Make a new toast\"\n"
    "do <string>      Same as make\n"
    "cut <ID>         Removes a toast from the list\n"
    "                 ie: toast cut 1\n"
    "rm <ID>          Same as cut\n"
    "done <ID>        Check the box of the toast ID\n"
    "                 ie: toast done 2\n"
    "version          Show version\n"
    "help             Show this help message\n"
  };
  assert(toast.get_help() == help_msg);
  
  assert(toast.get_usage() == "usage: toast [make <string>] [cut <number>] [done <number>] [clear]\n");
  
  assert(toast.get_toast_count() == 3);
#endif

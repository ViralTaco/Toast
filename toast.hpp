/*
 * Copyright (c) 2018 ViralTaco.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * <http://www.gnu.org/licenses/agpl.txt>
 */

#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

namespace vt {
struct data {
  std::string msg_;
  bool is_checked_;
  [[maybe_unused]] unsigned char padding[7];
  
  data(const bool is_checked, std::string msg)
  : msg_{msg}, is_checked_{is_checked} {
  }
};


class toast {
private:
  static constexpr char kFileName[]{"/.toast"};
  static constexpr char kUsage[]{
    "usage: toast [make <string>] [cut <number>] [done <number>] [clear]\n"
  };
  static constexpr char kVersion[]{
    "toast Copyright (c) 2018 ViralTaco\nVersion 1.0.1 (https://github.com/ViralTaco/Toast)\n"
  };
  static constexpr char kHelp[]{
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
  
  std::string filepath_;
  const std::string command_;
  std::string arg_;

  std::vector<data> data_vector_;
  
public:
  toast() noexcept(true) : filepath_{std::getenv("HOME")} {
    filepath_ += kFileName;
    data_vector_ = fill_data_vector();
  }
  
  toast(const std::string command) noexcept(true)
  : filepath_{std::getenv("HOME")}, command_{command} {
    filepath_ += kFileName;
    if (command_ == "clear") {
       cleartoast();  
    } else if (command_ == "--version" or command_ == "version") {
      std::cout << kVersion;
    } else if (command_ == "--help" or command_ == "help") {
      std::cout << kHelp;
    } else {
      print_usage();
    }
  }
  
  toast(const std::string command, const std::string arg) noexcept(true) 
  : filepath_{std::getenv("HOME")}, command_{command}, arg_{arg} {
    if (arg.length() >= 60) print_usage();
    
    filepath_ += kFileName;
    data_vector_ = fill_data_vector(); 
    exec_command();
  }
  
private:
  void exec_command() noexcept(true) {
    if (command_ == "make" or command_ == "do") {
      mktoast();
    } else if (command_ == "cut" or command_ == "rm") {
      rmtoast(to_size_t(arg_) - 1);
    } else if (command_ == "done") {
      checktoast(to_size_t(arg_) - 1);
    } else {
      print_usage(); 
    }
    data_vector_ = fill_data_vector();
  }
  
  size_t to_size_t(std::string& arg) const noexcept(false) {
    size_t result = 0;
    try { 
      result = std::stoul(arg);
    } catch ([[maybe_unused]] std::invalid_argument& e) {
      print_usage();
    }
    return result;
  }
  
  bool is_alpha(const std::string& str) const noexcept(true) {
    for (const char c: str) 
      if (not isalpha(c)) return false;
    return true;
  }
  
  bool does_exist(size_t id) const noexcept(false) {
    return id < data_vector_.size();
  }
  
  [[noreturn]] void no_such_toast() const noexcept(true) {
    std::cerr << "Toast: error: this toast doesn't exist";
    std::exit(EXIT_FAILURE);
  }
  
  void mktoast() const noexcept(false) {
    std::ofstream to_toast(filepath_, std::ios::app);
    to_toast << data_vector_.size() + 1 << " [ ] " << arg_ << std::endl;
  }
  
  void mkalltoasts(size_t i = 0) const noexcept(false) {
    std::ofstream new_toast(filepath_, std::ios::trunc); 
    for (const auto& d: data_vector_) {
      char mark = d.is_checked_ ? '*' : ' ';
      new_toast << ++i << " [" << mark << "] " << d.msg_ << std::endl;
    }
  }
  
  void rmtoast(size_t id) noexcept(false) {
    if (not does_exist(id)) no_such_toast();
    data_vector_.erase(data_vector_.begin() + static_cast<long>(id));
    data_vector_.shrink_to_fit();
    mkalltoasts();
  }
  
  void cleartoast() const noexcept(false) {
    std::ofstream clear_toast(filepath_, std::ofstream::out | std::ios::trunc);
    clear_toast.close();
  }
  
  void checktoast(size_t id) noexcept(false) {
    if (not does_exist(id)) no_such_toast();
    data_vector_.at(id).is_checked_ = true;
    mkalltoasts();
  }
  
  [[nodiscard]] std::vector<data> fill_data_vector() const noexcept(false) {
    std::ifstream filestream(filepath_, std::ios::in);
    std::vector<data> d;
    
    for (std::string line; std::getline(filestream, line);) {
      bool done = line.find('*') != std::string::npos;
      d.push_back(data(done, line.substr(line.find(']') + 2)));
    }
    return d;
  }
  
  [[noreturn]] void print_usage() const noexcept(true) {
    std::cout << kUsage;
    std::exit(EXIT_FAILURE);
  }

public: 
  void print(size_t i = 0) const noexcept(false) {
    for (const auto& d: data_vector_) {
      std::string mark = (d.is_checked_) ? "\x1b[92m*\x1b[0m" : " ";
      std::cout << ++i << " [" << mark << "] " << d.msg_ << '\n';
    }
  }
};
} // namespace vt

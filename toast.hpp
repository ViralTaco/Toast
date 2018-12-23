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
  std::string m_msg;
  bool m_is_checked;
  [[maybe_unused]] unsigned char padding[7];
  
  data(const bool is_checked, std::string msg)
  : m_msg{msg}, m_is_checked{is_checked} {
  }
};


class toast {
private:
  static constexpr char kUsage[]{
    "usage: toast [make <string>] [cut <number>] [done <number>] [clear]\n"
  };
  static constexpr char kFileName[]{"/.toast"};
  std::string m_filepath;
  const std::string m_command;
  std::string m_arg;

  std::vector<data> m_data_vector;
  
public:
  toast() noexcept(true) : m_filepath{std::getenv("HOME")} {
    m_filepath += kFileName;
    m_data_vector = fill_data_vector();
  }
  
  toast(const std::string command) noexcept(true)
  : m_filepath{std::getenv("HOME")}, m_command{command} {
    m_filepath += kFileName;
    if (m_command == "clear") {
       cleartoast();  
    } else {
      print_usage();
    }
  }
  
  toast(const std::string command, const std::string arg) noexcept(true) 
  : m_filepath{std::getenv("HOME")}, m_command{command}, m_arg{arg} {
    if (arg.length() >= 60) print_usage();
    
    m_filepath += kFileName;
    m_data_vector = fill_data_vector(); 
    exec_command();
  }
  
private:
  void exec_command() noexcept(true) {
    if (m_command == "make" or m_command == "do") {
      mktoast();
    } else if (m_command == "cut" or m_command == "rm") {
      rmtoast(std::stoul(m_arg) - 1);
    } else if (m_command == "done") {
      try { 
        checktoast(std::stoul(m_arg) - 1);
      } catch ([[maybe_unused]] std::invalid_argument& e) {
        print_usage();
      }
    } else {
      print_usage(); 
    }
    m_data_vector = fill_data_vector();
  }
  
  bool is_alpha(const std::string& str) const noexcept(true) {
    for (const char c: str) 
      if (not isalpha(c)) return false;
    return true;
  }
  
  bool does_exist(size_t id) const noexcept(false) {
    return id < m_data_vector.size();
  }
  
  [[noreturn]] void no_such_toast() const noexcept(true) {
    std::cerr << "Toast: error: this toast doesn't exist";
    std::exit(EXIT_FAILURE);
  }
  
  void mktoast() const noexcept(false) {
    std::ofstream to_toast(m_filepath, std::ios::app);
    to_toast << m_data_vector.size() + 1 << " [ ] " << m_arg << std::endl;
  }
  
  void mkalltoasts(size_t i = 0) const noexcept(false) {
    std::ofstream new_toast(m_filepath, std::ios::trunc); 
    for (const auto& d: m_data_vector) {
      char mark = d.m_is_checked ? '*' : ' ';
      new_toast << ++i << " [" << mark << "] " << d.m_msg << std::endl;
    }
  }
  
  void rmtoast(size_t id) noexcept(false) {
    if (not does_exist(id)) no_such_toast();
    m_data_vector.erase(m_data_vector.begin() + static_cast<long>(id));
    m_data_vector.shrink_to_fit();
    mkalltoasts();
  }
  
  void cleartoast() const noexcept(false) {
    std::ofstream clear_toast(m_filepath, std::ofstream::out | std::ios::trunc);
    clear_toast.close();
  }
  
  void checktoast(size_t id) noexcept(false) {
    if (not does_exist(id)) no_such_toast();
    m_data_vector.at(id).m_is_checked = true;
    mkalltoasts();
  }
  
  [[nodiscard]] std::vector<data> fill_data_vector() const noexcept(false) {
    std::ifstream filestream(m_filepath, std::ios::in);
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
    for (const auto& d: m_data_vector) {
      std::string mark = (d.m_is_checked) ? "\x1b[92m*\x1b[0m" : " ";
      std::cout << ++i << " [" << mark << "] " << d.m_msg << '\n';
    }
  }
};
} // namespace vt

/*
 * Copyright (c) 2018 Capobianco A.
 * SPDX-License-Identifier: MIT 
 * <http://www.opensource.org/licenses/MIT>
 */
#pragma once

#include "Bread.hpp"

#include <iostream>

class Toaster {
private:
  Bread bread_;
  std::string current_;
  size_t bread_count_;

public:
  Toaster(const std::string path, const std::string filename) noexcept(true)
    : bread_(path + "/" + filename)
    , current_(bread_.to_string()) 
    , bread_count_(bread_.get_bread_count()) 
  {}
  
  ~Toaster() {
    bread_.push_to_file();
  }
  
  bool add(const std::string& element) noexcept(false) {
    if (not is_valid(element)) return false;
    bread_ += element;
    return true;    
  }
  
  bool operator+=(const std::string& element) noexcept(false) {
    return this->add(element);
  }

  bool rm(const size_t& id) noexcept(true) {
    if (id - 1 >= bread_.get_bread_count()) return false;
    bread_.rm(id - 1);
    return true;
  }
  
  bool check(const size_t& id) noexcept(true) {
    if (id - 1 >= bread_.get_bread_count()) return false;
    bread_.check(id - 1);
    return true;
  }
  
  [[noreturn]] void clear() noexcept(false) {
    bread_.clear();
  }
  
private:
  bool is_valid(const std::string& arg) const noexcept(true) {
    return arg.length() < 60 
       and arg.find('*') == std::string::npos 
       and arg.find('\n') == std::string::npos;
  }

public:
  std::string get_current() const noexcept(true) {
    return bread_.to_string();
  }
  
  friend std::ostream& operator<<(std::ostream& out, const Toaster& kToasterRef) {
    return out << kToasterRef.get_current();
  }
};
/*
 * Copyright (c) 2018 Capobianco A.
 * SPDX-License-Identifier: MIT 
 * <http://www.opensource.org/licenses/MIT>
 */

#pragma once

#include "Element.hpp"

#include <vector>
#include <fstream>
#include <cstdlib>

class Bread {
private:  
  const std::string kPath_;
  std::vector<Element> elements_;

public: // Constructor
  Bread(const std::string path) noexcept(true) 
    : kPath_(path) {
    set_elements();
  }
  
private: // member functions
  void set_elements() noexcept(false) {
    std::ifstream filestream(kPath_, std::ios::in);
    
    for (std::string line; std::getline(filestream, line);) {
      bool done = line.find_first_of('*') != std::string::npos;
      *this += Element(done, line.substr(line.find(']') + 2));
    }
  }

public: // member functions
  void add(const std::string& element) noexcept(false) {
    elements_.push_back(Element(element));
  }
  
  void operator+=(const std::string& element) noexcept(false) {
    this->add(element);
  }
  
  void operator+=(const Element& element) noexcept(false) {
    this->elements_.push_back(element);
  }
  
  void rm(const size_t& id) noexcept(false) {
    elements_.erase(elements_.begin() + static_cast<long>(id));
    elements_.shrink_to_fit();
  }
  
  std::string to_string(size_t i = 0, std::string result = "") const noexcept(true) {
    for (const auto& element: elements_) {
      result += std::to_string(++i) + element.to_string();
    }
    return result;
  }
  
  void push_to_file() const noexcept(false) {
    std::ofstream push_toast(kPath_, std::ios::trunc);
    push_toast << this->to_string();
  }
  
  [[noreturn]] void clear() const noexcept(false) {
    std::ofstream clear_toast(kPath_, std::ofstream::out | std::ios::trunc);
    clear_toast.close();
    std::exit(EXIT_SUCCESS);
  }
  
public: // member functions (getters)
  size_t get_bread_count() const noexcept(true) {
    return elements_.size();
  }

public:
  void check(const size_t& id) noexcept(true) {
    elements_.at(id).check();
  }
};
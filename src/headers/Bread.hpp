/*
 * Copyright (c) 2018 Capobianco A.
 * SPDX-License-Identifier: MIT 
 * <http://www.opensource.org/licenses/MIT>
 */

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdlib>

struct Element {
public:
  bool is_checked_;
  std::string content_;
  std::string mark_;
  
  Element(bool is_checked, std::string content) noexcept 
    : is_checked_{is_checked}
    , content_{std::move(content)}
    , mark_{(is_checked) ? " [\x1b[92m*\x1b[0m] " : " [] "} 
  {}
  
  Element(std::string content) noexcept 
    : is_checked_{false}
    , content_{content}
    , mark_{" [] "}
  {}
  
  std::string to_string() const noexcept 
  {
    return mark_ + content_ + "\n";
  }
  
  void check() noexcept 
  {
    is_checked_ = true;
    mark_ = " [\x1b[92m*\x1b[0m] ";
  }
};

class Bread {
private:  
  const std::string kPath_;
  std::vector<Element> elements_;

public:
  Bread(std::string path) noexcept 
    : kPath_{ path }
  {
    set_elements();
  }
  
private:
  void set_elements() {
    std::ifstream filestream(kPath_, std::ios::in);
    
    // for all lines in the file
    for (std::string line; std::getline(filestream, line); ) {
      // if there is a '*' in the line the toast is done
      bool done{line.find_first_of('*') != std::string::npos};
      // The line is as follows: ID (int) [MARK (char)] STRING
      // We just want the string.
      elements_.push_back(Element(done, line.substr(line.find(']') + 2)));
    }
  }

public:
  void add(const std::string& element) {
    elements_.push_back(Element(element));
  }
  
  void operator += (const std::string& element) {
    this->add(element);
  }
  
  void rm(const std::size_t id) {
    elements_.erase(elements_.begin() + static_cast<long>(id));
    elements_.shrink_to_fit();
  }
  
  void push_to_file() const {
    std::ofstream push_toast(kPath_, std::ios::trunc);
    push_toast << this->to_string();
  }
  
  [[noreturn]] void clear() const {
    static constexpr auto kOpenMode{ std::ofstream::out | std::ios::trunc };
    std::ofstream clear_toast(kPath_, kOpenMode);
    clear_toast.close();
    std::exit(EXIT_SUCCESS);
  }
  
  void check(std::size_t id) noexcept {
    elements_.at(id).check();
  }
  
public:
  std::size_t get_bread_count() const noexcept {
    return elements_.size();
  }
  
  std::string to_string() const noexcept {
    std::string result;
    std::size_t i = 0;
    for (const auto& element: elements_) {
      result += std::to_string(++i) + element.to_string();
    }
    return result;
  }
  
  std::string to_string(const std::size_t index) const {
    return elements_.at(index).to_string();
  }
  
  std::pair<bool, std::string> at(const std::size_t index) const {
    const auto e{elements_.at(index)};
    return std::make_pair(e.is_checked_, e.content_);
    // This isn't good enough. 
    // at should return an actual ref to the element.
  }

  std::pair<bool, std::string> operator [] (const std::size_t index) const {
    return this->at(index);
  }
};

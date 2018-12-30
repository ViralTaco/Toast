/*
 * Copyright (c) 2018 Capobianco A.
 * SPDX-License-Identifier: MIT 
 * <http://www.opensource.org/licenses/MIT>
 */
#include <string>

class Element {
private:
  bool is_checked_;
  std::string content_;
  std::string mark_;
  
public:
  Element(bool is_checked, const std::string& content) noexcept(true) 
    : is_checked_(is_checked), content_(content)
    , mark_((is_checked) ? " [\x1b[92m*\x1b[0m] " : " [ ] ") {
  }
  
  Element(const std::string& content) noexcept(true) 
    : is_checked_(false), content_(content), mark_(" [ ] ") {
  }
  
  bool first() const noexcept(true) {
    return is_checked_;
  }
  
  std::string second() const noexcept(true) {
    return content_;
  }
  
  std::string to_string() const noexcept(true) {
    return mark_ + content_ + "\n";
  }
  
  void check() noexcept(true) {
    is_checked_ = true;
    mark_ = " [\x1b[92m*\x1b[0m] ";
  }
};
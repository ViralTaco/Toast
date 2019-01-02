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

public:
	Toaster(const std::string path_to_dir, const std::string filename) noexcept
		: bread_(path_to_dir + "/" + filename)
	{}
	
	~Toaster() 
	{
		bread_.push_to_file();
	}
	
	bool add(const std::string& content)
	{
		if (not is_valid(content)) return false;
		bread_ += content;
		return true;    
	}
	
	bool operator+=(const std::string& content)
	{
		return this->add(content);
	}

	bool rm(const size_t& id) noexcept 
	{
    // We want to make sure `id` isn't out of bounds
    // 0 - 1 would be a big number since we use size_t
		if (id - 1 >= get_count()) {
      return false;
    }
		bread_.rm(id - 1);
		return true;
	}
	
	bool check(const size_t& id) noexcept 
	{
		if (id - 1 >= get_count()) {
      return false;
    }
		bread_.check(id - 1);
		return true;
	}
	
	[[noreturn]] void clear() const 
	{
		bread_.clear();
	}
	
private:
	bool is_valid(const std::string& arg) const noexcept 
	{
		return arg.length() < 60 
			 and arg.find('*') == std::string::npos 
			 and arg.find('\n') == std::string::npos;
	}

public:
	std::string get_current() const noexcept 
	{
		return bread_.to_string();
	}
	
	std::string at(const size_t& id) const noexcept 
	{
		if (id - 1 >= get_count()) {
			return "";
		}
		return bread_.to_string(id - 1);
	}
	
	std::string operator[](const size_t& id) const noexcept
	{
		return this->at(id);
	}
	
	size_t get_count() const noexcept 
	{
		return bread_.get_bread_count();
	}
	
	std::pair<bool, std::string> get_element_at(const size_t& id) const noexcept 
	{
		if (id - 1 >= get_count()) {
			return std::make_pair(false, "");
		}
		return bread_[id - 1];
	}
	
	friend std::ostream& operator<<(std::ostream& out, const Toaster& kToasterRef) 
	{
		return out << kToasterRef.get_current();
	}
};

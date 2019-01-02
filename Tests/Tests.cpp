/*
 * Copyright (c) 2018 Capobianco A.
 * SPDX-License-Identifier: MIT 
 * <http://www.opensource.org/licenses/MIT>
 */

#include "../src/headers/Toaster.hpp"

#include <cstring>
#include <cassert>

std::string get_string() 
{
	std::string line;
	std::string result;
	std::ifstream test_file("./tests.txt", std::ifstream::in);
	
	for (int i = 0; std::getline(test_file, line); ++i) {
		if (i > 0) {
			result += "\n";
		}
		result += line;
	}
	
	test_file.close();
	
	return result;
}


int main() 
{
	std::ofstream clear_file("./tests.txt", std::ofstream::out | std::ios::trunc);
	clear_file.close();
	
	#define NEW_TEST(...) do { 					\
			Toaster test(".", "tests.txt"); \
			__VA_ARGS__ 										\
		} while (false)
	
	#define TRY(a) do {                                \
		if (!(get_string() == a)) {                      \
			std::cerr << "Expected: "                      \
								<< a << "\nRecieved: "               \
								<< get_string() << "\n";             \
								assert(false);                       \
		}                                                \
	} while (false)
	
	NEW_TEST(test += "some shit here";);
	TRY("1 [] some shit here");
	
	NEW_TEST(
		test += "\n";
		test += "*";
		test += "Some text longer than 60 characters. Some text longer than 60 characters.";
		test += "Another line of text";
	);
	
	TRY("1 [] some shit here\n2 [] Another line of text");
	
	bool is_valid = true;
	NEW_TEST(
		test.check(1);
		test.check(-1);
		is_valid = test.check(999);
	);
	TRY("1 [\x1b[92m*\x1b[0m] some shit here\n2 [] Another line of text");
	assert(is_valid == false);
	
	NEW_TEST(
		test.rm(1);
		test.rm(2); // this should not be executed !
		test.rm(-1); // neither should this
		test.rm('a'); // ...
		test.rm(0);
	);
	TRY("1 [] Another line of text");
	
	std::string current_view;
	NEW_TEST(
		current_view = test.get_current();
	);
	// Because Element::to_string() return \n at the end
	current_view.erase(std::find(current_view.begin(), current_view.end(), '\n'));
	
	TRY(current_view); 
	
	size_t count;
	{
		std::pair<bool, std::string> check_list;
		NEW_TEST(check_list = test.get_element_at(1););
		
		std::pair<bool, std::string> list = {false, "Another line of text"};
		
		assert(check_list == list);  
	}
	NEW_TEST(count = test.get_count(););
	assert(count == 1);
	
	{  
		std::pair<bool, std::string> check_list;
		std::pair<bool, std::string> list;
		NEW_TEST(test.rm(1););
		NEW_TEST(check_list = test.get_element_at(1););
		
		assert(list == check_list);
	}
	NEW_TEST(
		test.rm(1); 
		count = test.get_count();
	);
	assert(count == 0);
}

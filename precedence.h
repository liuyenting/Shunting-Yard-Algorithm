#ifndef PRECEDENCE_H
#define PRECEDENCE_H

#include <iostream>
#include <tuple>
#include <map>
#include <string>

// Custom types
typedef int (*function_ptr)(const int&, const int&);
enum associativity { left = 0, right = 1 };

// Storing all the valid operators
std::string operators = "";
// Store the precedence table itself
std::map<std::string, std::tuple<int, function_ptr, associativity> > rules;

// Dummy function for NULL operations
#pragma GCC diagnostic ignored "-Wreturn-type"
int null_function(const int& lhs, const int& rhs)
{}

// Add default rules
// Format: (operator, (precedence, function, associativity))
void initPrecedenceTable()
{
	rules.insert(std::make_pair("(", std::make_tuple(2, &null_function, left)));
	rules.insert(std::make_pair(")", std::make_tuple(2, &null_function, left)));

	// Unary
	rules.insert(std::make_pair("+", std::make_tuple(3, [](const int& lhs, const int& rhs){ return std::abs(rhs); }, right)));
	rules.insert(std::make_pair("-", std::make_tuple(3, [](const int& lhs, const int& rhs){ return -std::abs(rhs); }, right)));

	// Bitwise NOT
	rules.insert(std::make_pair("~", std::make_tuple(3, [](const int& lhs, const int& rhs){ return ~rhs; }, right)));
	// Logical NOT, non-zero -> 0, 0 -> 1
	rules.insert(std::make_pair("!", std::make_tuple(3, [](const int& lhs, const int& rhs){ return (rhs != 0) ? 0 : 1; }, right)));

	rules.insert(std::make_pair("*", std::make_tuple(5, [](const int&lhs, const int& rhs){ return lhs * rhs; }, left)));
	rules.insert(std::make_pair("/", std::make_tuple(5, [](const int&lhs, const int& rhs){ return lhs / rhs; }, left)));
	rules.insert(std::make_pair("%", std::make_tuple(5, [](const int&lhs, const int& rhs){ return lhs % rhs; }, left)));

	rules.insert(std::make_pair("+", std::make_tuple(6, [](const int&lhs, const int& rhs){ return lhs + rhs; }, left)));
	rules.insert(std::make_pair("-", std::make_tuple(6, [](const int&lhs, const int& rhs){ return lhs - rhs; }, left)));

	rules.insert(std::make_pair("<<", std::make_tuple(7, [](const int&lhs, const int& rhs){ return lhs << rhs; }, left)));
	rules.insert(std::make_pair(">>", std::make_tuple(7, [](const int&lhs, const int& rhs){ return lhs >> rhs; }, left)));

	rules.insert(std::make_pair("&", std::make_tuple(10, [](const int&lhs, const int& rhs){ return lhs & rhs; }, left)));
	rules.insert(std::make_pair("^", std::make_tuple(11, [](const int&lhs, const int& rhs){ return lhs ^ rhs; }, left)));
	rules.insert(std::make_pair("|", std::make_tuple(12, [](const int&lhs, const int& rhs){ return lhs | rhs; }, left)));
	rules.insert(std::make_pair("&&", std::make_tuple(13, [](const int&lhs, const int& rhs){ return (lhs && rhs) ? 1 : 0; }, left)));
	rules.insert(std::make_pair("||", std::make_tuple(14, [](const int&lhs, const int& rhs){ return (lhs || rhs) ? 1 : 0; }, left)));

	// Iterate through keys and link them to a single search pattern
	for(auto iterator = rules.begin(); iterator != rules.end(); ++iterator)
		operators += iterator->first;
}

#endif

#pragma once

#include <string>
#include <vector>

void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);

std::string ltrim_copy(std::string s);
std::string rtrim_copy(std::string s);
std::string trim_copy(std::string s);

void replace_all(std::string& str, const std::string& from, const std::string& to);
std::string replace_all_copy(std::string str, const std::string& from, const std::string& to);

bool ends_with(const std::string& str, const std::string& ending);
bool starts_with(const std::string& str, const std::string& starting);

std::vector<std::string> split(const std::string& input, const char& delim);

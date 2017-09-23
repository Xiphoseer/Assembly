#include "stringutil.hpp"

#include <algorithm>
#include <cctype>
#include <locale>

/* Adapted from
 * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring#217605
 *
 * Question by  Milan Babuškov (https://stackoverflow.com/users/14690/milan-babu%c5%a1kov)
 * Answer   by  Evan Teran     (https://stackoverflow.com/users/13430/evan-teran)
 *          and jotik          (https://stackoverflow.com/users/3919155/jotik)
 */

// trim from start (in place)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

// replace all substrings
void replace_all(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty()) return;

    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

// replace all substrings (copying)
std::string replace_all_copy(std::string str, const std::string& from, const std::string& to)
{
    replace_all(str, from, to);
    return str;
}

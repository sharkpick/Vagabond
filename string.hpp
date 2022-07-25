#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace string {

// binary search requires sorted vector
size_t search_strings(std::vector<std::string> s, std::string find) {
    size_t low = 0;
    size_t high = s.size();
    while (low < high) {
        size_t mid = (low+high)/2;
        if (s.at(mid) > find) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

bool is_float(std::string s) {
    auto sawDecimal = false;
    for (auto c: s) {
        if (c == '.') {
            if (sawDecimal) return false;
            sawDecimal = true;
        } else {
            if (!std::isdigit(c)) return false;
        }
    }
    return true;
}

bool is_integer(std::string s) {
    if (s.length() > 0 && s.front() == '-') {
        s.erase(s.begin());
    }
    for (auto c: s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

bool is_uint(std::string s) {
    if (s.length() > 0 && s.front() == '-') return false;
    for (auto c: s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

std::vector<std::string> sort(std::vector<std::string> s) {
    std::sort(s.begin(), s.end());
    return s;
}

std::string sort(std::string s) {
    std::sort(s.begin(), s.end());
    return s;
}

std::string file_buffer_to_string(std::vector<unsigned char> v) {
    return std::string(v.begin(), v.end());
}

std::string trim_space(std::string s) {
    size_t breakpoint;
    for (breakpoint = 0; breakpoint < s.length(); breakpoint++) {
        auto c = s.at(breakpoint);
        if (!std::isspace(c)) {
            break;
        }
    }
    s.erase(0, breakpoint);
    for (breakpoint = s.length() - 1; breakpoint >= 0; breakpoint--) {
        auto c = s.at(breakpoint);
        if (!std::isspace(c)) break;
    }
    s.erase(breakpoint, s.length());
    return s;
}

std::vector<std::string> split(std::string s, unsigned char delimiter) {
    std::vector<std::string> results;
    std::string token;
    for (auto &c: s) {
        if (c == delimiter) {
            results.push_back(token);
            token.clear();
            continue;
        } else {
            token.push_back(c);
        }
    }
    return results;
}

bool has_prefix(std::string s, std::string prefix) {
    if (prefix.length() > s.length()) return false;
    auto presumed_prefix = s.substr(0, prefix.length());
    return presumed_prefix == prefix;
}

bool has_suffix(std::string s, std::string suffix) {
    if (suffix.length() > s.length()) return false;
    auto presumed_suffix = s.substr(s.length() - suffix.length(), s.length());
    return presumed_suffix == suffix;
}


};
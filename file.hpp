#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <exception>

namespace file {

struct ErrFileNotFound : std::exception {
    std::string err;
    ErrFileNotFound() : err{"file not found"} {}
    ErrFileNotFound(std::string s) : err{s} {}
    const char* what() const noexcept {return err.c_str();}
};

std::vector<unsigned char> read_to_vector(std::string filename) {
    size_t size;
    try {
        size = std::filesystem::file_size(filename);
    } catch (std::filesystem::filesystem_error) {
        throw ErrFileNotFound("file "+filename+" not found");
    }
    std::vector<unsigned char> results;
    results.resize(size);
    auto file = std::ifstream(filename);
    if (file.is_open()) {
        file.read((char*)results.data(), size);
        file.close();
    } else {
        throw ErrFileNotFound("error opening "+filename);
    }
    return results;
}

std::string read_to_string(std::string filename) {
    std::vector<unsigned char> results = read_to_vector(filename);
    return std::string(results.begin(), results.end());
}

};
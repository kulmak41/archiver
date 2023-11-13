#pragma once

#include <exception>

class FileExistenceException : public std::exception {
public:
    const char* file_name;

    explicit FileExistenceException(const char* file_name);
};
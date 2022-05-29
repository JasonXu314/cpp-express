#ifndef UTILS_H
#define UTILS_H

#include <stdexcept>
#include <string>
#include <vector>

enum Method { GET, POST, PUT, DELETE, PATCH };

std::vector<std::string> split(const std::string& str, const std::string& delimiter);

Method parseMethod(const std::string& method);

#endif
#pragma once

#include <string>

namespace utils {
namespace path {
std::string ReplaceFileExtension(const std::string& filename,
                                 const std::string& new_extension);

std::string Basename(const std::string& path);
}  // namespace path
}  // namespace utils

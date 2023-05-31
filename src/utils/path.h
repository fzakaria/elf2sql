#pragma once

#include <string>

std::string replace_file_extension(const std::string& filename,
                                   const std::string& new_extension);

std::string basename(const std::string& path);
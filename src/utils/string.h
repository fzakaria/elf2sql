#pragma once

#include <string>
#include <vector>

namespace utils {

namespace string {

/**
 * Convert a string of hex characters to a vector of bytes.
 * @param hex uppercase or lowercase hex characters
 */
std::vector<uint8_t> HexToBytes(const std::string& hex);

}  // namespace string
}  // namespace utils
#include "src/utils/string.h"

namespace utils {
namespace string {

// https://stackoverflow.com/a/30606613
std::vector<uint8_t> HexToBytes(const std::string& hex) {
  std::vector<uint8_t> bytes;
  for (unsigned int i = 0; i < hex.length(); i += 2) {
    const std::string byteString = hex.substr(i, 2);
    const auto byte =
        static_cast<uint8_t>(strtol(byteString.c_str(), nullptr, 16));
    bytes.push_back(byte);
  }
  return bytes;
}

}  // namespace string
}  // namespace utils
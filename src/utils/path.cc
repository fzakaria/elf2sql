#include "src/utils/path.h"

namespace utils {
namespace path {
/**
 * For a given string replace the extension with the new one.
 * If no extension is present, simply append the new extension.
 */
std::string ReplaceFileExtension(const std::string& filename,
                                 const std::string& new_extension) {
  const size_t pos = filename.find_last_of('.');
  if (pos == std::string::npos) {
    return filename + "." + new_extension;
  }
  return filename.substr(0, pos + 1) + new_extension;
}

/**
 * Return the basename for a given string.
 */
std::string Basename(const std::string& path) {
  size_t const pos = path.find_last_of("/\\");
  if (pos == std::string::npos) {
    return path;
  }
  return path.substr(pos + 1);
}
}  // namespace path
}  // namespace utils
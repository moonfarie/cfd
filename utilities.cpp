#include "utilities.hpp"

#include <cstddef>
#include <fstream>
#include <stdexcept>

namespace cfd {

namespace utilities {

std::vector<char> read_file_as_char_vec(const std::filesystem::path& path) {
  std::ifstream file(path, std::ios::ate | std::ios::binary);

  if (not file.is_open()) {
    throw std::runtime_error("Failed to open file: " + path.string());
  }

  const size_t file_size = static_cast<size_t>(file.tellg());

  std::vector<char> buff(file_size);

  file.seekg(0);
  file.read(buff.data(), file_size);
  file.close();

  return buff;
}

}  // namespace utilities

}  // namespace cfd

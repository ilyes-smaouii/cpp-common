#include "misc.hpp"
#include <cstddef>
#include <cstring>
#include <memory>

namespace HLP {
namespace Misc {

std::size_t count_bits(std::uint64_t num) {
  std::size_t res{0};
  while (num > 0) {
    num >>= 1;
  }
  return res;
}

my_buffer::my_buffer(std::size_t buffer_size) {
  _data = std::shared_ptr<byte_t[]>(new byte_t[buffer_size]);
  std::memset(_data.get(), 0, buffer_size);
}

std::shared_ptr<byte_t[]> my_buffer::ptr() {
  return _data;
}

byte_t* my_buffer::data() {
  return _data.get();
}

const byte_t* my_buffer::data() const {
  return _data.get();
}

} // namespace Misc
} // namespace HLP
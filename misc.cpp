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

my_shared_buffer::my_shared_buffer(std::size_t buffer_size)
    : _size(buffer_size) {
  _data = std::make_shared<byte_t[]>(_size);
  // _data = std::shared_ptr<byte_t[]>(new byte_t[buffer_size]);
  std::memset(_data.get(), 0, _size);
}

std::shared_ptr<byte_t[]> my_shared_buffer::ptr() { return _data; }

byte_t *my_shared_buffer::data() { return _data.get(); }

const byte_t *my_shared_buffer::data() const { return _data.get(); }

std::size_t my_shared_buffer::getLength() const {
  return _size;
}

std::size_t my_shared_buffer::getSize() const {
  return _size;
}

byte_t *my_shared_buffer::getNthBytePtr(std::size_t pos) {
  return _data.get() + pos;
}

const byte_t *my_shared_buffer::getNthBytePtr(std::size_t pos) const {
  return _data.get() + pos;
}

} // namespace Misc
} // namespace HLP
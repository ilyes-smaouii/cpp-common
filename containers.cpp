#include <cstddef>
#include <cstring>
#include <memory>
#include <string.h>

#include "common-base.hpp"
#include "containers.hpp"

namespace HLP {
namespace Containers {

std::size_t count_bits(std::uint64_t num) {
  std::size_t res{0};
  while (num > 0) {
    num >>= 1;
    res++;
  }
  return res;
}

SharedNonFixedSizeBuffer::SharedNonFixedSizeBuffer(std::size_t buffer_size)
    : _size(buffer_size) {
  _data = std::make_shared<byte_t[]>(_size);
  // _data = std::shared_ptr<byte_t[]>(new byte_t[buffer_size]);
  std::memset(_data.get(), 0, _size);
}

std::shared_ptr<byte_t[]> SharedNonFixedSizeBuffer::ptr() { return _data; }

byte_t *SharedNonFixedSizeBuffer::data() { return _data.get(); }

const byte_t *SharedNonFixedSizeBuffer::data() const { return _data.get(); }

std::size_t SharedNonFixedSizeBuffer::getLength() const { return _size; }

std::size_t SharedNonFixedSizeBuffer::getSize() const { return _size; }

byte_t *SharedNonFixedSizeBuffer::getNthBytePtr(std::size_t pos) {
  return _data.get() + pos;
}

const byte_t *SharedNonFixedSizeBuffer::getNthBytePtr(std::size_t pos) const {
  return _data.get() + pos;
}

std::string construct_string_with_max_len(const char *start_char,
                                          std::size_t max_len) {
  if (!start_char) {
    return {};
  } else {
    return std::string{start_char, strnlen(start_char, max_len)};
  }
}

} // namespace Containers
} // namespace HLP
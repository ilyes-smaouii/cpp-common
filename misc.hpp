#include "common-common.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace HLP {
namespace Misc {

/*
  Counts how many bits, at minimum, it takes to represent an unsigned number
*/
std::size_t count_bits(std::uint64_t num);

/*
  Structure for dealing with fixed_size buffers
 */
template <std::size_t BUFFER_SIZE>
struct FIXED_SIZE_BUFFER {
  std::array<byte_t, BUFFER_SIZE> _data{};

  // TO-DO : add relevant methods
  void set_to_zero() { _data.fill(0); }
  constexpr static std::size_t get_buffer_size() { return BUFFER_SIZE; }
  byte_t *data() { return _data.data(); }
  const byte_t *data() const { return _data.data(); }
};

} // namespace Misc
} // namespace HLP
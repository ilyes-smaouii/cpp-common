#include "common-common.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

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

struct my_buffer{
  std::shared_ptr<byte_t[]> _data{};

  my_buffer(std::size_t buffer_size);
  byte_t *data();
  std::shared_ptr<byte_t[]> ptr();
  const byte_t *data() const;
  template<typename DT>
  DT dataAs() {
    return reinterpret_cast<DT>(_data.get());
  }
  template<typename DT>
  DT dataAs() const {
    return const_cast<DT>(reinterpret_cast<DT>(_data.get()));
  }
};

} // namespace Misc
} // namespace HLP
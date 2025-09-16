#include "misc.hpp"

namespace HLP {
namespace Misc {

std::size_t count_bits(std::uint64_t num) {
  std::size_t res{0};
  while (num > 0) {
    num >>= 1;
  }
  return res;
}

} // namespace Misc
} // namespace HLP
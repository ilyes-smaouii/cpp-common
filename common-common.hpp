#pragma once

#include <cstdint>
#include <concepts>

namespace HLP {
using byte_t = std::uint8_t;

template <typename T>
concept IsUsableAsBuffer = true && requires(T t) {
  { t.data() } -> std::convertible_to<byte_t *>;
  { t.data() } -> std::convertible_to<const byte_t *>;
  { t.getSize() } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept IsUsableAsFixedSizeBuffer = IsUsableAsBuffer<T> && requires(T t) {
  T::getSize();
};

template <typename T>
concept IsUsableAsNonFixedSizeBuffer = IsUsableAsBuffer<T>;

} // namespace HLP
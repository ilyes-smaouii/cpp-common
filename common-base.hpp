#pragma once

#include <cstdint>

namespace HLP {
using byte_t = std::uint8_t;

template <typename... Ts>
struct last_type {};

template <typename T>
struct last_type<T> {
  using type = T;
};

template <typename T0, typename T1, typename... Ts>
struct last_type<T0, T1, Ts...> : last_type<T1, Ts...> {};

template <typename... Ts>
using last_type_t = typename last_type<Ts...>::type;

} // namespace HLP
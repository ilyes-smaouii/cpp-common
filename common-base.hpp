#pragma once

#include <cstdint>
#include <functional>
#include <iterator>
#include <type_traits>

namespace HLP {
using byte_t = std::uint8_t;

template <typename... Ts> struct last_type {};

template <typename T> struct last_type<T> {
  using type = T;
};

template <typename T0, typename T1, typename... Ts>
struct last_type<T0, T1, Ts...> : last_type<T1, Ts...> {};

template <typename... Ts> using last_type_t = typename last_type<Ts...>::type;

template <typename T>
concept Hashable = requires() { std::hash<T>(); };

template <typename T>
std::size_t
hashOnList(std::initializer_list<T> values,
           std::function<std::size_t(T)> hash_func = std::hash<T>()) {
  std::size_t hash_value = 0;
  for (auto const &value : values) {
    hash_value ^=
        hash_func(value) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
  }
  return hash_value;
}

} // namespace HLP
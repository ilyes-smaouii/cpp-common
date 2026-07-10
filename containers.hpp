#pragma once

#include <array>
#include <cmath>
#include <csetjmp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "common-base.hpp"

namespace HLP {
namespace Containers {

template <typename T>
concept IsUsableAsBuffer = true && requires(T t) {
  { std::data(t) } -> std::convertible_to<const byte_t *>;
  { std::size(t) } -> std::convertible_to<std::size_t>;
  // { t.template dataAs<byte_t *>() } -> std::same_as<byte_t *>;
};

template <typename T>
concept HasStaticSize = requires(T t) { T::size(); };

template <typename T>
concept IsUsableAsStaticSizeBuffer = IsUsableAsBuffer<T> && HasStaticSize<T>;

template <typename T>
concept IsUsableAsRuntimeSizeBuffer = IsUsableAsBuffer<T> && !HasStaticSize<T>;

template <typename T>
concept IsUsableAsDynamicSizeBuffer =
    IsUsableAsRuntimeSizeBuffer<T> &&
    requires(T t, std::size_t size) { t.resize(size); };

/*
Structure for dealing with fixed_size buffers
*/
template <std::size_t BUFFER_SIZE> struct ArrayBuffer {
  std::array<byte_t, BUFFER_SIZE> _data{};

  // TO-DO : add relevant methods
  void set_to_zero() { _data.fill(0); }
  constexpr static std::size_t get_buffer_size() { return BUFFER_SIZE; }
  byte_t *data() { return _data.data(); }
  const byte_t *data() const { return _data.data(); }
  template <typename DT>
    requires(std::is_pointer_v<DT>)
  DT dataAs() {
    return reinterpret_cast<DT>(_data.get());
  }
  template <typename DT>
    requires(std::is_pointer_v<DT>)
  DT dataAs() const {
    return const_cast<DT>(reinterpret_cast<DT>(_data.get()));
  }
  constexpr static std::size_t size() { return BUFFER_SIZE; }
  // TO-DO : test these functions below
  template <typename DT>
    requires(std::is_pointer_v<DT>)
  DT getNthBytePtrAs(std::size_t index) {
    return reinterpret_cast<DT>(this->data() + index);
  };
  // Also, is the second one really necessary, or is just redundant ? I'm not
  // sure
  template <typename DT>
    requires(std::is_pointer_v<DT>)
  const DT getNthBytePtrAs(std::size_t index) const {
    return reinterpret_cast<const DT>(this->data() + index);
  };
};

struct SharedNonFixedSizeBuffer {
public:
  /*
   * Allocates buffer_size bytes in memory, and 0-initializes them
   */
  SharedNonFixedSizeBuffer() = delete;
  SharedNonFixedSizeBuffer(std::size_t buffer_size);
  ~SharedNonFixedSizeBuffer() = default;
  //
  std::shared_ptr<byte_t[]> ptr();
  byte_t *data();
  const byte_t *data() const;
  std::size_t getLength() const;
  std::size_t size() const;
  template <typename DT> DT dataAs() {
    return reinterpret_cast<DT>(_data.get());
  }
  template <typename DT> DT dataAs() const {
    return const_cast<DT>(reinterpret_cast<DT>(_data.get()));
  }
  byte_t *getNthBytePtr(std::size_t pos);
  const byte_t *getNthBytePtr(std::size_t pos) const;
  template <typename DT> DT getNthBytePtrAs(std::size_t pos) {
    return reinterpret_cast<DT>(_data.get() + pos);
  }
  template <typename DT> DT getNthBytePtrAs(std::size_t pos) const {
    return const_cast<DT>(reinterpret_cast<DT>(_data.get() + pos));
  }

protected:
  std::shared_ptr<byte_t[]> _data{};
  const std::size_t _size{};
};

static_assert(IsUsableAsRuntimeSizeBuffer<SharedNonFixedSizeBuffer>,
              "Error : SharedNonFixedSizeBuffer doesn't satisfy "
              "IsUsableAsRuntimeSizeBuffer requirements !");
static_assert(IsUsableAsDynamicSizeBuffer<std::vector<byte_t>>,
              "Error : std::vector<byte_t> should be satisfy "
              "IsUsableAsModifiableSizeBuffer requirements !");

/*
  Counts how many bits, at minimum, it takes to represent an unsigned number
*/
std::size_t count_bits(std::uint64_t num);

std::string construct_string_with_max_len(const char *start_char,
                                          std::size_t max_len);

} // namespace Containers
} // namespace HLP
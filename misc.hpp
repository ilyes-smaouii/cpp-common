#pragma once

#include <concepts>
#include <cstdint>
#include <format>
#include <memory>
#include <random>
#include <type_traits>
#include <unordered_set>

#include "logging.hpp"

namespace HLP {

template <std::unsigned_integral T> T truncateUint64(std::uint64_t value) {
  return static_cast<T>(value);
}

template <typename T>
  requires std::is_unsigned_v<T>
class UuidGeneratorAux {
  static_assert(
      sizeof(T) <= sizeof(std::uint64_t),
      "Error : integral type T can't have size bigger than std::uint64_t's !");

public:
  using uuid_t = T;

  UuidGeneratorAux() : m_seed(std::random_device{}()), m_rng(m_seed), m_ids() {
    // TO-DO : initialization
  }
  UuidGeneratorAux(UuidGeneratorAux const &) = default;
  UuidGeneratorAux(UuidGeneratorAux &&) = default;
  UuidGeneratorAux &operator=(UuidGeneratorAux const &) = default;
  UuidGeneratorAux &operator=(UuidGeneratorAux &&) = default;

  T getNextUuid() {
    constexpr std::size_t MAX_ATTEMPTS{10};
    bool inserted = false;
    T new_uuid = m_rng(); // not entirely sure I need this
    inserted = (m_ids.insert(new_uuid)).second;
    std::size_t attempts = 1;
    [[unlikely]] while (!inserted) {
      HLP_LOG_ERROR(std::format(
          "Generated Uuid {} has already been generated before !", new_uuid));
      new_uuid = m_rng();
      inserted = (m_ids.insert(new_uuid)).second;
      [[unlikely]] if (++attempts > MAX_ATTEMPTS) {
        THROW_ERROR_MSG("Max attempts to genrate Uuid exceeded !");
      }
    }
    return new_uuid;
  }

protected:
  std::mt19937_64::result_type m_seed{};
  std::mt19937_64 m_rng{};
  std::unordered_set<T> m_ids{};

private:
};

template <typename T> class SingletonUuidGenerator {
public:
  SingletonUuidGenerator(SingletonUuidGenerator const &other) = delete;
  SingletonUuidGenerator(SingletonUuidGenerator &&other) = delete;
  SingletonUuidGenerator &
  operator=(SingletonUuidGenerator const &other) = delete;
  SingletonUuidGenerator &operator=(SingletonUuidGenerator &&other) = delete;

  static std::shared_ptr<SingletonUuidGenerator<T>> getInstancePtr() {
    if (!SingletonUuidGenerator::m_instance_ptr) {
      SingletonUuidGenerator<T>();
      // TO-DO : make a choice here
      // auto ptr = new SingletonUuidGenerator();
      // SingletonUuidGenerator::m_instance_ptr =
      //     std::shared_ptr<SingletonUuidGenerator<T>>(ptr);
      SingletonUuidGenerator::m_instance_ptr =
          std::make_shared<SingletonUuidGenerator>();
    }
    return SingletonUuidGenerator::m_instance_ptr;
  }

  T getNextUuid() { return m_uuid_gen_ptr->getNextUuid(); }

protected:
  SingletonUuidGenerator() {
    m_uuid_gen_ptr = std::make_shared<UuidGeneratorAux<T>>();
  }

  // TO-DO : figure out Singleton mechanics/check correction
  static inline std::shared_ptr<SingletonUuidGenerator<T>> m_instance_ptr;
  std::shared_ptr<UuidGeneratorAux<T>> m_uuid_gen_ptr;

private:
};

} // namespace HLP
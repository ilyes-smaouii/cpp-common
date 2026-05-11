#pragma once

#include <cstdint>
#include <format>
#include <memory>
#include <random>
#include <type_traits>
#include <unordered_set>

#include "logging.hpp"

namespace HLP {

template <typename T>
  requires std::is_integral_v<T>
class UuidGeneratorAux {
  static_assert(
      sizeof(T) <= sizeof(std::uint64_t),
      "Error : integral type T can't have size bigger than std::uint64_t's !");

public:
  using uuid_t = T;

  UuidGeneratorAux() : m_ids(), m_rng(std::random_device{}()) {
    // TO-DO : initialization
  }
  UuidGeneratorAux(UuidGeneratorAux const &) = default;
  UuidGeneratorAux(UuidGeneratorAux &&) = default;
  UuidGeneratorAux &operator=(UuidGeneratorAux const &) = default;
  UuidGeneratorAux &operator=(UuidGeneratorAux &&) = default;

  T getNextUuid() {
    constexpr std::size_t MAX_ATTEMPTS{10};
    std::uint64_t new_uuid;
    bool inserted = false;
    new_uuid = m_rng();
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
  std::unordered_set<T> m_ids{};
  std::mt19937_64 m_rng{};

private:
};

template <typename T> class SingletonUuidGenerator {
public:
  SingletonUuidGenerator(SingletonUuidGenerator &other) = delete;
  SingletonUuidGenerator &operator=(SingletonUuidGenerator &other) = delete;

  static std::shared_ptr<SingletonUuidGenerator<T>> &getInstancePtr() {
    if (!m_instance_ptr) {
      m_instance_ptr = std::make_shared<SingletonUuidGenerator<T>>();
    }
    return *m_instance_ptr;
  }

  T getNextUuid() { return m_uuid_gen_ptr->getNextUuid(); }

protected:
  SingletonUuidGenerator() {
    m_uuid_gen_ptr = std::make_shared<UuidGeneratorAux<T>>();
  }

private:
  static std::shared_ptr<SingletonUuidGenerator<T>> m_instance_ptr;
  static std::shared_ptr<UuidGeneratorAux<T>> m_uuid_gen_ptr;
};

} // namespace HLP
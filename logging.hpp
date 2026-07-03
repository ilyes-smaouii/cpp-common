#pragma once

#include <concepts>
#include <cstdint>
#include <format>
#include <iostream>
#include <ostream>
#include <vector>

#include "common-base.hpp"

namespace HLP {
// should I nest another namespace inside this one ?
namespace Log {

enum class LogLevel {
  DEBUG = 1,
  INFO = 2,
  WARNING = 3,
  ERROR = 4,
  CRITICAL = 5
};

static LogLevel CURRENT_LOG_LEVEL{LogLevel::DEBUG};

static std::ostream *DEFAULT_OSTREAM = &std::cout;

class NullOstream : public std::ostream {
  // TO-DO : improve class

public:
  NullOstream() : std::ostream(nullptr) {}
  NullOstream(const NullOstream &) = delete;
  NullOstream &operator=(const NullOstream &) = delete;
  ~NullOstream() override = default;

  static NullOstream &getInstance() {
    static NullOstream instance;
    return instance;
  }

  template <typename T>
  NullOstream &operator<<(const T &) {
    return *this;
  }
};

template <typename... Args>
std::vector<std::string> getLogEntries(LogLevel log_lvl, Args... args) {
  if (log_lvl < CURRENT_LOG_LEVEL) {
    return {};
  }

  // std::cout << "[debugging]" << std::endl; // [debugging]

  std::vector<std::string> log_entries;
  for (const auto &arg : {args...}) {
    log_entries.push_back(std::format("{}", arg));
  }
  return log_entries;
}

template <typename... Args>
void printLogEntries(LogLevel log_lvl, Args... args) {
  auto log_entries = getLogEntries(log_lvl, args...);
  for (const auto &entry : log_entries) {
    (*DEFAULT_OSTREAM) << entry << " ";
  }
  (*DEFAULT_OSTREAM) << std::endl;
}

constexpr char HEX_DIGITS[17] = "0123456789ABCDEF";

template <typename T>
  requires std::unsigned_integral<T>
void quickHexToString(T byte, char *buffer);

template <>
inline void quickHexToString<std::uint8_t>(std::uint8_t hex, char *buffer) {
  buffer[0] = HEX_DIGITS[(hex >> 4) & 0x0F];
  buffer[1] = HEX_DIGITS[hex & 0x0F];
  buffer[2] = '\0';
}

template <>
inline void quickHexToString<std::uint16_t>(std::uint16_t hex, char *buffer) {
  buffer[0] = HEX_DIGITS[(hex >> 12) & 0x0F];
  buffer[1] = HEX_DIGITS[(hex >> 8) & 0x0F];
  buffer[2] = HEX_DIGITS[(hex >> 4) & 0x0F];
  buffer[3] = HEX_DIGITS[hex & 0x0F];
  buffer[4] = '\0';
}

template <>
inline void quickHexToString<std::uint32_t>(std::uint32_t hex, char *buffer) {
  buffer[0] = HEX_DIGITS[(hex >> 28) & 0x0F];
  buffer[1] = HEX_DIGITS[(hex >> 24) & 0x0F];
  buffer[2] = HEX_DIGITS[(hex >> 20) & 0x0F];
  buffer[3] = HEX_DIGITS[(hex >> 16) & 0x0F];
  buffer[4] = HEX_DIGITS[(hex >> 12) & 0x0F];
  buffer[5] = HEX_DIGITS[(hex >> 8) & 0x0F];
  buffer[6] = HEX_DIGITS[(hex >> 4) & 0x0F];
  buffer[7] = HEX_DIGITS[hex & 0x0F];
  buffer[8] = '\0';
}

template <>
inline void quickHexToString<std::uint64_t>(std::uint64_t hex, char *buffer) {
  buffer[0] = HEX_DIGITS[(hex >> 60) & 0x0F];
  buffer[1] = HEX_DIGITS[(hex >> 56) & 0x0F];
  buffer[2] = HEX_DIGITS[(hex >> 52) & 0x0F];
  buffer[3] = HEX_DIGITS[(hex >> 48) & 0x0F];
  buffer[4] = HEX_DIGITS[(hex >> 44) & 0x0F];
  buffer[5] = HEX_DIGITS[(hex >> 40) & 0x0F];
  buffer[6] = HEX_DIGITS[(hex >> 36) & 0x0F];
  buffer[7] = HEX_DIGITS[(hex >> 32) & 0x0F];
  buffer[8] = HEX_DIGITS[(hex >> 28) & 0x0F];
  buffer[9] = HEX_DIGITS[(hex >> 24) & 0x0F];
  buffer[10] = HEX_DIGITS[(hex >> 20) & 0x0F];
  buffer[11] = HEX_DIGITS[(hex >> 16) & 0x0F];
  buffer[12] = HEX_DIGITS[(hex >> 12) & 0x0F];
  buffer[13] = HEX_DIGITS[(hex >> 8) & 0x0F];
  buffer[14] = HEX_DIGITS[(hex >> 4) & 0x0F];
  buffer[15] = HEX_DIGITS[hex & 0x0F];
  buffer[16] = '\0';
}

#define HLP_LOG_DEBUG(...)                                                     \
  HLP::Log::printLogEntries(HLP::Log::LogLevel::DEBUG, __VA_ARGS__)
#define HLP_LOG_INFO(...)                                                      \
  HLP::Log::printLogEntries(HLP::Log::LogLevel::INFO, __VA_ARGS__)
#define HLP_LOG_WARNING(...)                                                   \
  HLP::Log::printLogEntries(HLP::Log::LogLevel::WARNING, __VA_ARGS__)
#define HLP_LOG_ERROR(...)                                                     \
  HLP::Log::printLogEntries(HLP::Log::LogLevel::ERROR, __VA_ARGS__)
#define HLP_LOG_CRITICAL(...)                                                  \
  HLP::Log::printLogEntries(HLP::Log::LogLevel::CRITICAL, __VA_ARGS__)

#define LOCATION_INFO()                                                        \
  std::format("[{}:{} - {}()]", __FILE__, __LINE__, __func__)

#define THROW_ERROR_MSG(MSG)                                                   \
  throw std::runtime_error(std::format("{} - {}", LOCATION_INFO(), MSG))

} // namespace Log
} // namespace HLP

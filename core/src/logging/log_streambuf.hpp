#pragma once

#include <iostream>

#include <logging/level.hpp>

#include <logging/spdlog.hpp>

namespace logging {

class MessageBuffer final : public std::streambuf {
 public:
  MessageBuffer(Level level);
  ~MessageBuffer() = default;

  void StartTskvValueEncoding() noexcept { tskv_value_encode_ = true; }
  void EndTskvValueEncoding() noexcept { tskv_value_encode_ = false; }

 protected:
  std::streamsize xsputn(const char_type* s, std::streamsize n) override;
  int_type overflow(int_type c) override;

 public:
  bool tskv_value_encode_;
  spdlog::details::log_msg msg;
};

}  // namespace logging

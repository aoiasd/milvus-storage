// Copyright 2023 Zilliz
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "milvus-storage/format/vortex/vortex_io_trace.h"

#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <mutex>
#include <thread>

#include "vortex_bridge.h"

namespace milvus_storage::vortex {

#ifdef MILVUS_STORAGE_WITH_VORTEX_IO_TRACE
namespace {

constexpr auto kDefaultTraceInterval = std::chrono::seconds(60);

std::chrono::seconds GetTraceInterval() {
  const char* value = std::getenv("MILVUS_STORAGE_VORTEX_IO_TRACE_INTERVAL_SECONDS");
  if (value == nullptr) {
    return kDefaultTraceInterval;
  }

  char* end = nullptr;
  const auto seconds = std::strtoll(value, &end, 10);
  if (end == value || *end != '\0' || seconds <= 0) {
    return kDefaultTraceInterval;
  }
  return std::chrono::seconds(seconds);
}

class AutoIOTraceSession {
  public:
  AutoIOTraceSession() : interval_(GetTraceInterval()) {
    ResetIOTrace();
    reporter_ = std::thread([this]() { ReportLoop(); });
  }

  ~AutoIOTraceSession() {
    {
      std::lock_guard lock(mutex_);
      stopped_ = true;
    }
    wakeup_.notify_all();
    reporter_.join();

    if (IsIOTraceEnabled()) {
      PrintIOTrace();
      DisableIOTrace();
    }
  }

  private:
  void ReportLoop() {
    std::unique_lock lock(mutex_);
    while (!wakeup_.wait_for(lock, interval_, [this]() { return stopped_; })) {
      lock.unlock();
      if (IsIOTraceEnabled()) {
        PrintAndResetIOTrace();
      }
      lock.lock();
    }
  }

  const std::chrono::seconds interval_;
  std::mutex mutex_;
  std::condition_variable wakeup_;
  bool stopped_{false};
  std::thread reporter_;
};

AutoIOTraceSession g_auto_io_trace_session;

}  // namespace
#endif

void ResetIOTrace() { ffi::reset_io_trace_ffi(); }

void PrintIOTrace() { ffi::print_io_trace_ffi(); }

void PrintAndResetIOTrace() { ffi::print_and_reset_io_trace_ffi(); }

void DisableIOTrace() { ffi::disable_io_trace_ffi(); }

bool IsIOTraceEnabled() { return ffi::io_trace_enabled_ffi(); }

}  // namespace milvus_storage::vortex
